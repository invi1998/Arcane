// Copyright INVI1998


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AttributeSet.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(this->AttributeSet))
	{
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());			// 广播生命值改变
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());		// 广播最大生命值改变
		OnManaChanged.Broadcast(AuraAttributeSet->GetMana());				// 广播法力值改变
		OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());			// 广播最大法力值改变
	}
}
