// Copyright INVI1998


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Arcane/ArcaneLogChannels.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    // 一但主角或者Enemy的AbilityActorInfo被设置，就代表他们的AbilitySystemComponent已经被初始化了，这时候我们就可以绑定EffectApplied委托了

    // 绑定EffectApplied委托，该委托在效果应用到目标时调用
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

    // 这个委托只会在服务端被调用，如果我们想在客户端也能调用，那么我们需要将这个委托声明为Client，Reliable
    // 客户端RPC被设计成在服务端调用，然后服务端再广播到所有客户端，在客户端执行

}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag, bool bActivate)
{
	// 1：调用激活被动效果的委托
	ActivatePassiveEffectDelegate.Broadcast(AbilityTag, bActivate);
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

            // 同时，在赋予能力的时候，我们还需要将能力状态设置为已装备
            AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_State_Equipped);

            // 将能力添加到AbilitySystemComponent中
            GiveAbility(AbilitySpec);   // 添加能力
            // GiveAbilityAndActivateOnce(AbilitySpec);    // 添加并激活能力
        }
	}
    bStartupAbilitiesGiven = true;    // 标记已经添加了能力
    AbilitiesGivenDelegate.Broadcast();    // 广播给UI，告诉UI能力已经添加，可以显示了
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability.GetDefaultObject(), 1);
		GiveAbilityAndActivateOnce(AbilitySpec);   // 添加能力并激活一次
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    // 1：检测输入的标签是否有效
    if (InputTag.IsValid())
    {
        FScopedAbilityListLock AbilityLock(*this);	// 创建一个作用域锁，锁住AbilitySystemComponent的能力列表

    	// 2：获取所有的激活的能力(激活的能力是指那些可以被激活的能力) 这取决于AbilitySystemComponent的激活策略
		TArray<FGameplayAbilitySpec> AbilitiesOfTheActivatable = GetActivatableAbilities();

		// 3：遍历所有的激活的能力
		for (FGameplayAbilitySpec& Spec : AbilitiesOfTheActivatable)
		{
            // 4：检测能力的输入标签是否与输入的标签相同，这里采用的是精确匹配
            if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
            {
               // 5： 判断能力是否已经激活
                if (Spec.IsActive())
                {
                    // InvokeReplicatedEvent是一个复制事件，它会在服务端调用，然后在客户端广播。这是我们能对按下和释放事件进行预测响应的原因。
                    InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());    // 调用复制事件
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
        FScopedAbilityListLock AbilityLock(*this);	// 创建一个作用域锁，锁住AbilitySystemComponent的能力列表

        // 3：遍历所有的激活的能力
        for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
        {
            // 4：检测能力的输入标签是否与输入的标签相同，这里采用的是精确匹配，及检测能力是否已经激活
            if (Spec.DynamicAbilityTags.HasTagExact(InputTag) && Spec.IsActive())
            {
				// 5：释放输入标签
                AbilitySpecInputReleased(Spec);    // 释放输入标签

                // 为能力输入标签的释放调用复制事件，我们在蓝图中就可以使用WaitInputRelease来等待这个事件
                // 比如对于一个持续性技能，我们可以在按下输入标签时激活技能，然后在释放输入标签时停止技能
                InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());    // 调用复制事件
            }
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    // 1：检测输入的标签是否有效
    if (InputTag.IsValid())
    {
        FScopedAbilityListLock AbilityLock(*this);	// 创建一个作用域锁，锁住AbilitySystemComponent的能力列表

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

FGameplayTag UAuraAbilitySystemComponent::GetAbilityStateTag(const FGameplayAbilitySpec& Spec)
{
	for (const FGameplayTag& Tag : Spec.DynamicAbilityTags)
	{
		// 如果能力的标签包含了"Abilities.State"，那么返回这个标签
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.State"))))
		{
			return Tag;
		}
	}

	return FGameplayTag();    // 返回一个空的标签
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagByAbilityTag(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* Spec = FindAbilitySpecByTag(AbilityTag))
	{
		return GetAbilityStateTag(*Spec);    // 返回技能状态标签
	}

	return FGameplayTag();    // 返回一个空的标签
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagByAbilityTag(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* Spec = FindAbilitySpecByTag(AbilityTag))
	{
		return GetAbilityInputTagBySpec(*Spec);    // 返回技能输入标签
	}

	return FGameplayTag();    // 返回一个空的标签
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagByInputTag(const FGameplayTag& InputTag)
{
    FScopedAbilityListLock AbilityLock(*this);	// 创建一个作用域锁，锁住AbilitySystemComponent的能力列表
	// 1：遍历所有的激活的能力
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		// 2：获取能力的输入标签
		FGameplayTag AbilityInputTag = GetAbilityInputTagBySpec(Spec);
		if (AbilityInputTag.IsValid() && AbilityInputTag.MatchesTagExact(InputTag))
		{
			// 3：获取能力的标签
			return GetAbilityTagBySpec(Spec);
		}
	}

	return FGameplayTag();    // 返回一个空的标签
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::FindAbilitySpecByTag(const FGameplayTag& AbilityTag)
{
    // 检索之前，需要锁住AbilitySystemComponent的能力列表
    FScopedAbilityListLock AbilityLock(*this);	// 创建一个作用域锁，锁住AbilitySystemComponent的能力列表

	// 1：遍历所有的激活的能力
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		// 2：获取能力的标签
		for (const FGameplayTag& Tag : Spec.Ability->AbilityTags)
		{
			// 3：判断能力的标签是否与输入的标签相同
			if (Tag.MatchesTag(AbilityTag))
			{
				return &Spec;    // 返回这个能力
			}
		}
	}

	return nullptr;    // 返回空指针
}

bool UAuraAbilitySystemComponent::IsEmptySlot(const FGameplayTag& SlotTag)
{
    // 在我们遍历所有的激活的能力之前，我们需要锁住AbilitySystemComponent的能力列表
    FScopedAbilityListLock AbilityLock(*this);	// 创建一个作用域锁，锁住AbilitySystemComponent的能力列表
	// 1：遍历所有的激活的能力
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		// 2：获取能力的输入标签
		if (AbilityHasSlot(&Spec, SlotTag))
		{
			return false;    // 返回false
		}
	}

	return true;    // 返回true
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec* Spec) const
{
	// 1：检测能力是否有效
	if (Spec)
	{
		// 2：获取能力的标签 (通过获取指定Tag的AbilityInfo来判断是否是被动技能)
		const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

        if (AbilityInfo)
        {
        	const FGameplayTag AbilityTypeTag = AbilityInfo->FindAbilityInfoByTag(GetAbilityTagBySpec(*Spec)).AbilityTypeTag;    // 获取技能类型标签

	        if (AbilityTypeTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Type_Passive))
	        {
        		return true;    // 返回true
			}
        }
	
	}

	return false;    // 返回false
}

bool UAuraAbilitySystemComponent::AbilityHasAnyInputTag(const FGameplayAbilitySpec* Spec)
{
    return Spec->DynamicAbilityTags.HasTagExact(FGameplayTag::RequestGameplayTag(FName("InputTag")));    // 返回是否有输入标签
}

void UAuraAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& SlotTag)
{
    ClearSlot(&AbilitySpec);    // 清除槽
    AbilitySpec.DynamicAbilityTags.AddTag(SlotTag);    // 添加槽标签
}

FGameplayTag UAuraAbilitySystemComponent::GetCoolDownTagByAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = FindAbilitySpecByTag(AbilityTag))
	{
        return Spec->Ability->GetCooldownTags()->First();
	}

	return FGameplayTag();    // 返回一个空的标签
}

int32 UAuraAbilitySystemComponent::GetAbilityLevelByTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = FindAbilitySpecByTag(AbilityTag))
	{
		return Spec->Level;    // 返回技能等级
	}

	return 0;    // 返回0
}

void UAuraAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
    Payload.EventTag = AttributeTag;    // 设置事件标签
    Payload.EventMagnitude = 1;    // 设置事件幅度
    
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);    // 发送游戏事件到Actor

    if (GetAvatarActor()->Implements<UPlayerInterface>())
    {
    	IPlayerInterface::Execute_AddAttributePoint(GetAvatarActor(), -1);    // 减少一个属性点
	}
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
    // 判断角色是否拥有可分配的属性点
    if (GetAvatarActor()->Implements<UPlayerInterface>())
    {
	    const int32 AttrPoints = IPlayerInterface::Execute_GetAttributePoint(GetAvatarActor());
		if (AttrPoints > 0)
		{
            // 如果存在可分配的属性点，那么就可以升级属性，但是该操作只能在服务端进行
            ServerUpgradeAttribute(AttributeTag);	// 服务端升级属性
		}
    }
}

float UAuraAbilitySystemComponent::GetManaCostByAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = FindAbilitySpecByTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(Spec->Ability))
		{
			return AuraAbility->GetManaCost(Spec->Level);    // 返回技能的法力消耗
		}
	}

	return 0.0f;    // 返回0
}

void UAuraAbilitySystemComponent::UpdateAbilityStateTags(int32 NewLevel)
{
	// 获取AbilityInfo
    UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

    // 遍历AbilityInfo中的所有能力，它里面有该能力的等级需求
    for (const FAuraAbilityInfo &Info : AbilityInfo->AbilitiesInformation)
    {
    	if (NewLevel >= Info.LevelRequired && Info.AbilityTag.IsValid())
    	{
    		// 如果角色的等级大于等于技能的等级需求，那么就判断该技能是否已经添加
			FGameplayAbilitySpec* Spec = FindAbilitySpecByTag(Info.AbilityTag);
			if (!Spec)
			{
				// 如果技能还没有添加，那么就将技能状态设置为已解锁
                FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);
                AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_State_UnLocked);   // 设置技能状态为已解锁
                GiveAbility(AbilitySpec);    // 添加技能，但是不激活
                // 添加技能并激活
                // GiveAbilityAndActivateOnce(AbilitySpec);    // 添加并激活技能
                MarkAbilitySpecDirty(AbilitySpec);    // 标记技能为脏，这样在下一帧就会更新技能状态
                ClientUpdateAbilityStateTags(Info.AbilityTag, FAuraGameplayTags::Get().Abilities_State_UnLocked, 1);    // 通知客户端技能状态改变
			}
    	}
	}
}

void UAuraAbilitySystemComponent::ServerEquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& SlotTag)
{
	// 1：检查当前Tag是否有效
	if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByTag(AbilityTag))
	{
        const FGameplayTag& PreviousSlotTag = GetAbilityInputTagBySpec(*AbilitySpec);	// 获取之前的槽标签
        const FGameplayTag& StatusTag = GetAbilityStateTag(*AbilitySpec);    // 获取技能状态标签
        const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();    // 获取AuraGameplayTags

        const bool bStatusValid = StatusTag.IsValid() && (StatusTag.MatchesTagExact(AuraTags.Abilities_State_UnLocked) || StatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_State_Equipped));
        if (bStatusValid)
        {
            if (!IsEmptySlot(SlotTag))
            {
            	// 如果槽不为空
            	FGameplayAbilitySpec* PreSlotAbilitySpec = FindAbilitySpecByTag(GetAbilityTagByInputTag(SlotTag));    // 通过输入标签查找技能

                // 判断是否是被动技能
                if (PreSlotAbilitySpec)
                {
                    const FGameplayTag& PreSlotAbilityTag = GetAbilityTagBySpec(*PreSlotAbilitySpec);    // 获取技能标签
	                // 判断两个技能是否相同
					if (PreSlotAbilitySpec->Ability == AbilitySpec->Ability)
					{
						// 如果是相同的技能，那么就直接返回
                        return;
					}

                    // 判断是否是被动技能，同时两个技能也都得相同，同是被动技能
                    if (IsPassiveAbility(PreSlotAbilitySpec))
					{
                        MulticastActivatePassiveEffect(PreSlotAbilityTag, false);    // 广播取消被动技能
                        DeactivatePassiveAbilitiesDelegate.Broadcast(PreSlotAbilityTag);    // 广播取消被动技能
					}

                    ClearSlot(PreSlotAbilitySpec);    // 清除槽
                    ClientEquipAbility(PreSlotAbilityTag, AuraTags.Abilities_State_Eligible, FGameplayTag::EmptyTag, SlotTag);    // 通知客户端装备技能
				}
            }

            if (!AbilityHasAnyInputTag(AbilitySpec))
            {
            	// 如果技能没有输入标签，那么就说明该技能还没有添加，未激活
				if (IsPassiveAbility(AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);    // 尝试激活能力
                    MulticastActivatePassiveEffect(AbilityTag, true);    // 广播激活被动技能
				}
            }

	        ClearSlot(AbilitySpec);    // 清除槽

            // 将新的槽标签添加到技能的DynamicAbilityTags中
            AbilitySpec->DynamicAbilityTags.AddTag(SlotTag);    // 添加槽标签

            if (StatusTag.MatchesTagExact(AuraTags.Abilities_State_UnLocked))
            {
            	// 如果技能状态是已解锁，那么就设置技能状态为已装备
				AbilitySpec->DynamicAbilityTags.RemoveTag(AuraTags.Abilities_State_UnLocked);    // 移除技能状态标签
				AbilitySpec->DynamicAbilityTags.AddTag(AuraTags.Abilities_State_Equipped);    // 添加技能状态标签
			}

            MarkAbilitySpecDirty(*AbilitySpec);    // 标记技能为脏，这样在下一帧就会更新技能状态
            // 通知客户端技能状态改变
        }

        ClientEquipAbility(AbilityTag, AuraTags.Abilities_State_Equipped, SlotTag, PreviousSlotTag);    // 通知客户端装备技能
        // OnAbilitySlotChangeDelegate.Broadcast(this, PreviousSlotTag, SlotTag);    // 广播技能槽改变
	}
}

void UAuraAbilitySystemComponent::ClientEquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlotTag)
{
    AbilitySlotChangedDelegate.Broadcast(AbilityTag, Status, SlotTag, PreviousSlotTag);    // 广播技能槽改变
    OnAbilitySlotChangeDelegate.Broadcast(this, PreviousSlotTag, SlotTag);    // 广播技能槽改变
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
    const FGameplayTag& InputTag = GetAbilityInputTagBySpec(*Spec);    // 获取输入标签
    Spec->DynamicAbilityTags.RemoveTag(InputTag);    // 移除输入标签

    // 修改技能状态为已解锁
    Spec->DynamicAbilityTags.RemoveTag(FAuraGameplayTags::Get().Abilities_State_Equipped);    // 移除技能状态标签
    Spec->DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_State_UnLocked);    // 添加技能状态标签
}

void UAuraAbilitySystemComponent::ClearAbilityOnSlot(const FGameplayTag& SlotTag)
{
    FScopedAbilityListLock AbilityLock(*this);	// 创建一个作用域锁，锁住AbilitySystemComponent的能力列表

	// 1：遍历所有的激活的能力
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		// 2：获取能力的输入标签
		if (AbilityHasSlot(&Spec, SlotTag))
		{
			// 3：清除槽
			ClearSlot(&Spec);    // 清除槽
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag)
{
	return Spec->DynamicAbilityTags.HasTagExact(SlotTag);    // 判断技能是否有槽
}

bool UAuraAbilitySystemComponent::GetDescriptionByTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());

	if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByTag(AbilityTag))
	{
		// 说明技能已经添加或者已经解锁
		if (UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level);
			return true;
		}
		else
		{
			OutDescription = FString();
			OutNextLevelDescription = FString();
			return false;
		}
	}
    else
    {
        if (AbilityTag.IsValid())
        {
	        OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoByTag(AbilityTag).LevelRequired);
        }
        
        OutNextLevelDescription = FString();
        return false;
    }
}

void UAuraAbilitySystemComponent::ServerSpendSkillPoint_Implementation(const FGameplayTag& AbilityTag)
{
	// 检查当前可用技能点数是否大于0
	const int32 SkillPoints = GetAvatarActor()->Implements<UPlayerInterface>() ? IPlayerInterface::Execute_GetSkillPoint(GetAvatarActor()) : 0;
    if (SkillPoints <= 0) return;

    const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();    // 获取AuraGameplayTags

    // 检查当前Tag是否有效
    if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecByTag(AbilityTag))
    {
        // 如果角色实现了PlayerInterface，那么就可以消耗技能点
        if (GetAvatarActor()->Implements<UPlayerInterface>())
        {
            IPlayerInterface::Execute_AddSkillPoint(GetAvatarActor(), -1);    // 减少一个技能点
        }

    	// 检查当前Tag状态是否为锁定状态，如果是锁定状态，则不能升级
    	FGameplayTag StateTag = GetAbilityStateTag(*AbilitySpec);
        if (StateTag.IsValid() && StateTag.MatchesTagExact(AuraTags.Abilities_State_Eligible))
        {
        	AbilitySpec->DynamicAbilityTags.RemoveTag(AuraTags.Abilities_State_Eligible);    // 移除技能状态标签
			AbilitySpec->DynamicAbilityTags.AddTag(AuraTags.Abilities_State_UnLocked);    // 添加技能状态标签
            StateTag = AuraTags.Abilities_State_UnLocked;    // 设置技能状态标签
		}

    	if (StateTag.IsValid() &&( StateTag.MatchesTagExact(AuraTags.Abilities_State_UnLocked) || StateTag.MatchesTagExact(AuraTags.Abilities_State_Equipped)))
        {
        	// 如果技能已经解锁，那么就升级技能
			AbilitySpec->Level++;    // 升级技能
		}
        
        // 通知客户端技能状态改变
        ClientUpdateAbilityStateTags(AbilityTag, StateTag, AbilitySpec->Level);
        MarkAbilitySpecDirty(*AbilitySpec);    // 标记技能为脏，这样在下一帧就会更新技能状态
        
	}
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

    if (!bStartupAbilitiesGiven)
	{
        bStartupAbilitiesGiven = true;
    	AbilitiesGivenDelegate.Broadcast();
	}
   
}

void UAuraAbilitySystemComponent::ClientUpdateAbilityStateTags_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StateTag, const int32 AbilityLevel)
{
	// 通过AbilityStatusChangedDelegate委托广播技能状态改变
	AbilityStatusChangedDelegate.Broadcast(StateTag, AbilityTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
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
