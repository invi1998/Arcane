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
	// 广播初始值
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	// Do nothing
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	// 广播技能信息

	if (!GetAuraASC() || !GetAuraASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastAbilityDelegate;
	BroadcastAbilityDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)->void
		{
			// 通过AbilityTag获取AbilityInfo
			const FGameplayTag tag = AuraAbilitySystemComponent->GetAbilityTagBySpec(AbilitySpec);
			FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(tag);	// 通过Tag查找AbilityInfo

			// 获取输入Tag
			const FGameplayTag inputTag = AuraAbilitySystemComponent->GetAbilityInputTagBySpec(AbilitySpec);
			AbilityInfo.InputTag = inputTag;	// 设置输入Tag

			// 广播委托
			AbilityInfoDelegate.Broadcast(AbilityInfo);	// 广播能力信息
		}
	);
	GetAuraASC()->ForEachAbility(BroadcastAbilityDelegate);	// 对每个能力进行广播

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
