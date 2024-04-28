// Copyright INVI1998


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"

#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                             const FGameplayTag& InCooldownTag)
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
	AbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitForCooldownChange, &UWaitCooldownChange::CooldownChanged);

	// 冷却何时开始，（当添加一个新的基于持续时间的GameplayEffect时）
	// 每当添加一个新的基于持续时间的GameplayEffect时，客户端和服务器都会调用此委托
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitForCooldownChange, &UWaitCooldownChange::OnActiveEffectAdded);

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

void UWaitCooldownChange::CooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)	// 如果冷却时间为0，结束任务
	{
		CooldownEnd.Broadcast(0);
	}
}

void UWaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());	// 查询是否有CooldownTag

		// 为什么GetActiveEffectsTimeRemaining返回的是一个float数组？
		// 因为该函数返回所有匹配的GameplayEffect的剩余时间，而不是只返回一个
		TArray<float> TimeRemainingArray = AbilitySystemComponent->GetActiveEffectsTimeRemaining(Query);	// 获取剩余时间
		// 冷却开始
		if (TimeRemainingArray.Num() > 0)
		{
			// 返回冷却数组里的最大值作为我们的冷却时间
			float CooldownTime = TimeRemainingArray[0];
			for (int i = 1; i < TimeRemainingArray.Num(); i++)
			{
				CooldownTime = FMath::Max(CooldownTime, TimeRemainingArray[i]);
			}
			CooldownEnd.Broadcast(CooldownTime);
		}
	}
}
