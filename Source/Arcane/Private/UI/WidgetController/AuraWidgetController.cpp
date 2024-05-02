// Copyright INVI1998


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& InParams)
{
	PlayerController = InParams.PlayerController;
	PlayerState = InParams.PlayerState;
	AbilitySystemComponent = InParams.AbilitySystemComponent;
	AttributeSet = InParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues(const FGameplayTag& Tag)
{
	// �㲥��ʼֵ
	
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	// Do nothing
}
