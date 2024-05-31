// Copyright INVI1998


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SkillPointChangeDelegate.Broadcast(GetAuraPS()->GetSkillPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatus, int32 NewLevel)->void
	{
		if (AbilityInformation)
		{
			FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(AbilityTag);
			AbilityInfo.StateTag = AbilityStatus;	// 设置技能状态标签
			AbilityInfoDelegate.Broadcast(AbilityInfo);
		}
	});

	// 添加技能状态改变的委托
	GetAuraPS()->OnSkillPointChangedDelegate.AddLambda([this](int32 SkillPoints)->void
	{
		SkillPointChangeDelegate.Broadcast(SkillPoints);	// 广播技能点改变
	});

	// 添加技能槽改变的委托
	GetAuraASC()->AbilitySlotChangedDelegate.AddUObject(this, &USpellMenuWidgetController::OnAbilitySlotChange);
}

void USpellMenuWidgetController::SpendSkillPoint(const FGameplayTag& AbilityTag)
{
	GetAuraASC()->ServerSpendSkillPoint(AbilityTag);
}

void USpellMenuWidgetController::EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag)
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	const FGameplayTag& SelectedAbilityType = AbilityInformation->FindAbilityInfoByTag(AbilityTag).AbilityTypeTag;	// 获取技能类型标签
	const FGameplayTag SelectedAbilityStatus = GetAuraASC()->GetStatusTagByAbilityTag(AbilityTag);

	if (SelectedAbilityStatus.IsValid())
	{
		// 如果当前技能状态标签是锁定状态, 则不执行任何操作
		if (SelectedAbilityStatus.MatchesTagExact(AuraTags.Abilities_State_Locked)) return;

		GetAuraASC()->ServerEquipAbility(AbilityTag, SlotTag);
	}

	
}

void USpellMenuWidgetController::OnAbilitySlotChange(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& OldSlotTag)
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;	// 上一个槽信息
	LastSlotInfo.StateTag = AuraTags.Abilities_State_UnLocked;	// 设置技能状态标签为解锁状态
	LastSlotInfo.InputTag = OldSlotTag;	// 设置输入标签为旧槽标签
	LastSlotInfo.AbilityTag = FGameplayTag::EmptyTag;	// 设置技能标签为空标签
	AbilityInfoDelegate.Broadcast(LastSlotInfo);	// 广播技能信息

	FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(AbilityTag);	// 通过技能标签查找技能信息
	AbilityInfo.StateTag = StatusTag;	// 设置技能状态标签
	AbilityInfo.InputTag = SlotTag;	// 设置输入标签
	AbilityInfoDelegate.Broadcast(AbilityInfo);	// 广播技能信息
}

