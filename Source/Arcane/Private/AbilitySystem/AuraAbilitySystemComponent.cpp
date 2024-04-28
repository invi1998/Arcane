// Copyright INVI1998


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Arcane/ArcaneLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    // 一但主角或者Enemy的AbilityActorInfo被设置，就代表他们的AbilitySystemComponent已经被初始化了，这时候我们就可以绑定EffectApplied委托了

    // 绑定EffectApplied委托，该委托在效果应用到目标时调用
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

    // 这个委托只会在服务端被调用，如果我们想在客户端也能调用，那么我们需要将这个委托声明为Client，Reliable
    // 客户端RPC被设计成在服务端调用，然后服务端再广播到所有客户端，在客户端执行

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
        // 创建能力
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability.GetDefaultObject(), 1);

        if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
        {
            // 如果是AuraGameplayAbility，那么设置StartupInputTag
            AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
            // DynamicAbilityTags是一个标签集合，它包含了一些标签，这些标签可以在运行时动态添加或移除。
            // 在这个上下文中，DynamicAbilityTags是AbilitySpec的一个成员变量，用于存储与Ability相关的标签。
            // 简单来说，就是比如我在开始的时候给一个角色添加了一个能力，这个能力可以被左键点击触发，所以我将左键输入标签添加到了这个能力的DynamicAbilityTags中。
            // 然后在游戏运行过程中，我可以卸载或者更换这个输入标签，改为右键输入标签，这样这个能力就可以被右键点击触发了。

            // 将能力添加到AbilitySystemComponent中
            GiveAbility(AbilitySpec);   // 添加能力
            // GiveAbilityAndActivateOnce(AbilitySpec);    // 添加并激活能力
        }
	}
    bStartupAbilitiesGiven = true;    // 标记已经添加了能力
    AbilitiesGivenDelegate.Broadcast(this);    // 广播给UI，告诉UI能力已经添加，可以显示了
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    // 1：检测输入的标签是否有效
    if (InputTag.IsValid())
    {
    	// 2：获取所有的激活的能力(激活的能力是指那些可以被激活的能力) 这取决于AbilitySystemComponent的激活策略
		TArray<FGameplayAbilitySpec> AbilitiesOfTheActivatable = GetActivatableAbilities();

		// 3：遍历所有的激活的能力
		for (FGameplayAbilitySpec& Spec : AbilitiesOfTheActivatable)
		{
            // 4：检测能力的输入标签是否与输入的标签相同，这里采用的是精确匹配
            if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
            {
               // 5： 判断能力是否已经激活
                if (!Spec.IsActive())
                {
                    UKismetSystemLibrary::PrintString(this, "Ability is not active", true, true, FLinearColor::Green, 5.0f);
                	// 6：尝试激活能力
					TryActivateAbility(Spec.Handle);    // 尝试激活能力
				}
                else
                {
	                UKismetSystemLibrary::PrintString(this, "Ability is already active", true, true, FLinearColor::Red, 5.0f);
                }
            }
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    // 1：检测输入的标签是否有效
    if (InputTag.IsValid())
    {
        // 2：获取所有的激活的能力(激活的能力是指那些可以被激活的能力) 这取决于AbilitySystemComponent的激活策略

        // 3：遍历所有的激活的能力
        for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
        {
            // 4：检测能力的输入标签是否与输入的标签相同，这里采用的是精确匹配
            if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
            {
				// 5：释放输入标签
                AbilitySpecInputReleased(Spec);    // 释放输入标签
            }
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    // 1：检测输入的标签是否有效
    if (InputTag.IsValid())
    {
        // 2：获取所有的激活的能力(激活的能力是指那些可以被激活的能力) 这取决于AbilitySystemComponent的激活策略
        TArray<FGameplayAbilitySpec> AbilitiesOfTheActivatable = GetActivatableAbilities();

        // 3：遍历所有的激活的能力
        for (FGameplayAbilitySpec& Spec : AbilitiesOfTheActivatable)
        {
            // 4：检测能力的输入标签是否与输入的标签相同，这里采用的是精确匹配
            if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
            {
				AbilitySpecInputPressed(Spec);      // 按住输入标签
                // 5： 判断能力是否已经激活
                if (!Spec.IsActive())
                {
                    // 6：尝试激活能力
                    TryActivateAbility(Spec.Handle);    // 尝试激活能力
                }
            }
        }
    }
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
    FScopedAbilityListLock AbilityLock(*this);	// 创建一个作用域锁，锁住AbilitySystemComponent的能力列表
    // 该锁的作用是在作用域结束时自动解锁，这样就不用担心忘记解锁了
    // 而且在作用域内，锁住了AbilitySystemComponent的能力列表，这样就不会有其他线程对其进行修改

	// 2：遍历所有的激活的能力
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		// 3：调用委托
		if (!Delegate.ExecuteIfBound(Spec))
		{
			UE_LOG(LogArcane, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagBySpec(const FGameplayAbilitySpec& Spec)
{
    if (Spec.Ability)
	{
		for (const FGameplayTag& Tag : Spec.Ability.Get()->AbilityTags)
		{
            // 如果能力的标签包含了"Abilities"，那么返回这个标签
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}

    return FGameplayTag();    // 返回一个空的标签
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityInputTagBySpec(const FGameplayAbilitySpec& Spec)
{
	for (const FGameplayTag& Tag : Spec.DynamicAbilityTags)
	{
		// 如果能力的标签包含了"Input"，那么返回这个标签
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();    // 返回一个空的标签
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

    if (!bStartupAbilitiesGiven)
	{
        bStartupAbilitiesGiven = true;
    	AbilitiesGivenDelegate.Broadcast(this);
	}
   
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
    FGameplayTagContainer AssetTagContainer;    // 创建一个GameplayTagContainer
    // 通过绑定委托，获取到GameplayTag，然后将其添加到AbilitySystemComponent的AssetTags中
    EffectSpec.GetAllAssetTags(AssetTagContainer);

    EffectAssetTags.Broadcast(AssetTagContainer);    // 广播这些Tag到UI

    for (const FGameplayTag& Tag : AssetTagContainer)
    {
        // TODO: 广播这些Tag到UI
        // UKismetSystemLibrary::PrintString(this, Tag.ToString(), true, true, FLinearColor::Gray, 5.0f);
    }
}
