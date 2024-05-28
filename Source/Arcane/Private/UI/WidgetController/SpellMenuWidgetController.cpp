// Copyright INVI1998


#include "UI/WidgetController/SpellMenuWidgetController.h"

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
			AbilityInfo.StateTag = AbilityStatus;	// ���ü���״̬��ǩ
			AbilityInfoDelegate.Broadcast(AbilityInfo);
		}
	});

	// ��Ӽ���״̬�ı��ί��
	GetAuraPS()->OnSkillPointChangedDelegate.AddLambda([this](int32 SkillPoints)->void
	{
		SkillPointChangeDelegate.Broadcast(SkillPoints);	// �㲥���ܵ�ı�
	});
}

void USpellMenuWidgetController::SpendSkillPoint(const FGameplayTag& AbilityTag)
{
	GetAuraASC()->ServerSpendSkillPoint(AbilityTag);
}

