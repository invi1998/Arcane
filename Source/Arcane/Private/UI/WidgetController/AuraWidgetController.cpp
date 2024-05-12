// Copyright INVI1998


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InParams)
{
	PlayerController = InParams.PlayerController;
	PlayerState = InParams.PlayerState;
	AbilitySystemComponent = InParams.AbilitySystemComponent;
	AttributeSet = InParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
	// �㲥��ʼֵ
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	// Do nothing
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	// �㲥������Ϣ

	if (!GetAuraASC() || !GetAuraASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastAbilityDelegate;
	BroadcastAbilityDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)->void
		{
			// ͨ��AbilityTag��ȡAbilityInfo
			const FGameplayTag tag = AuraAbilitySystemComponent->GetAbilityTagBySpec(AbilitySpec);
			FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(tag);	// ͨ��Tag����AbilityInfo

			// ��ȡ����Tag
			const FGameplayTag inputTag = AuraAbilitySystemComponent->GetAbilityInputTagBySpec(AbilitySpec);
			AbilityInfo.InputTag = inputTag;	// ��������Tag

			// �㲥ί��
			AbilityInfoDelegate.Broadcast(AbilityInfo);	// �㲥������Ϣ
		}
	);
	GetAuraASC()->ForEachAbility(BroadcastAbilityDelegate);	// ��ÿ���������й㲥

}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (!AuraPlayerController)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	
	return AuraPlayerController;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (!AuraPlayerState)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	
	return AuraPlayerState;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (!AuraAbilitySystemComponent)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	
	return AuraAbilitySystemComponent;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (!AuraAttributeSet)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	
	return AuraAttributeSet;
}
