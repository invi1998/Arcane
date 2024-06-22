// Copyright INVI1998


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Kismet/KismetSystemLibrary.h"

TArray<FGameplayTag> UWaitCooldownChange::ActiveCooldownTags;

UWaitCooldownChange* UWaitCooldownChange::WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UWaitCooldownChange* WaitForCooldownChange = NewObject<UWaitCooldownChange>();
	WaitForCooldownChange->ASC = AbilitySystemComponent;
	WaitForCooldownChange->CooldownTag = InCooldownTag;

	if (!IsValid(AbilitySystemComponent) || !InCooldownTag.IsValid())
	{
		WaitForCooldownChange->EndTask();
		return nullptr;
	}

	// 冷却何时结束（CooldownTag何时被移除）
	// 我们可以通过订阅RegiterGameplayTagEvent来监听CooldownTag的变化
	// 注意：以为这是一个静态函数，没有this指针，所以我们需要使用上面创建的WaitForCooldownChange对象来AddUObject
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitForCooldownChange, &UWaitCooldownChange::CooldownTagChanged);

	// 冷却何时开始，（当添加一个新的基于持续时间的GameplayEffect时）
	// 每当添加一个新的基于持续时间的GameplayEffect时，客户端和服务器都会调用此委托
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitForCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

	// 同时，我们还需要监听技能槽的变化，因为技能槽的变化的时候，如果有技能处于冷却状态，我们需要更新冷却时间到新的技能槽上
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraAbilitySystemComponent->OnAbilitySlotChangeDelegate.AddUObject(WaitForCooldownChange, &UWaitCooldownChange::OnInputTagChanged);
	}

	return WaitForCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	// 移除监听
	if (IsValid(ASC))
	{
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}

	SetReadyToDestroy();	// 设置为准备销毁，动作完全结束后调用，这样就能自由删除该操作，并且取消在游戏实例中的注册
	MarkAsGarbage();		// 标记为垃圾，等待GC回收
}

void UWaitCooldownChange::OnInputTagChanged(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& OldTag, const FGameplayTag& NewTag) const
{
	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		// 先获取技能槽的AbilityTag
		const FGameplayTag AbilityTag = AuraAbilitySystemComponent->GetAbilityTagByInputTag(NewTag);
		const FGameplayTag TempCooldownTag = AuraAbilitySystemComponent->GetCoolDownTagByAbilityTag(AbilityTag);

		if (TempCooldownTag.IsValid() && ActiveCooldownTags.Contains(TempCooldownTag) && CooldownTag.MatchesTagExact(TempCooldownTag))
		{
			const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());	// 查询是否有CooldownTag

			// 为什么GetActiveEffectsTimeRemaining返回的是一个float数组？
			// 因为该函数返回所有匹配的GameplayEffect的剩余时间，而不是只返回一个
			TArray<float> TimeRemainingArray = AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);	// 获取剩余时间
			// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("TimeRemainingArray.Num() = %d"), TimeRemainingArray.Num()), true, false, FLinearColor::Red, 5.0f);
			// 冷却开始
			if (TimeRemainingArray.Num() > 0)
			{
				// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("TimeRemainingArray[0] = %f"), TimeRemainingArray[0]), true, false, FLinearColor::Blue, 5.0f);
				// 返回冷却数组里的最大值作为我们的冷却时间
				float CooldownTime = TimeRemainingArray[0];
				for (int i = 1; i < TimeRemainingArray.Num(); i++)
				{
					CooldownTime = FMath::Max(CooldownTime, TimeRemainingArray[i]);
				}
				CooldownStart.Broadcast(CooldownTime);
			}
			else
			{
				ActiveCooldownTags.Remove(CooldownTag);
			}
		}
	}
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)	// 如果冷却时间为0，结束任务
	{
		CooldownEnd.Broadcast(0);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);		// 从EffectSpec中获取所有的AssetTags，这些标签是在编辑器中添加的

	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);	// 从EffectSpec中获取所有的GrantedTags，这些标签是在运行时添加的

	ActiveCooldownTags.AddUnique(CooldownTag);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());	// 查询是否有CooldownTag

		// 为什么GetActiveEffectsTimeRemaining返回的是一个float数组？
		// 因为该函数返回所有匹配的GameplayEffect的剩余时间，而不是只返回一个
		TArray<float> TimeRemainingArray = AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);	// 获取剩余时间
		// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("TimeRemainingArray.Num() = %d"), TimeRemainingArray.Num()), true, false, FLinearColor::Red, 5.0f);
		// 冷却开始
		if (TimeRemainingArray.Num() > 0)
		{
			// UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("TimeRemainingArray[0] = %f"), TimeRemainingArray[0]), true, false, FLinearColor::Blue, 5.0f);
			// 返回冷却数组里的最大值作为我们的冷却时间
			float CooldownTime = TimeRemainingArray[0];
			for (int i = 1; i < TimeRemainingArray.Num(); i++)
			{
				CooldownTime = FMath::Max(CooldownTime, TimeRemainingArray[i]);
			}
			CooldownStart.Broadcast(CooldownTime);
		}
		else
		{
			if (ActiveCooldownTags.Contains(CooldownTag))	// 如果冷却时间为0，结束任务
			{
				// 说明冷却时间已经结束，将它从ActiveCooldownTags中移除
				ActiveCooldownTags.Remove(CooldownTag);
			}
		}
	}
}
