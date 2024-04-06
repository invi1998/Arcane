// Copyright INVI1998


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AttributeSet.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(this->AttributeSet))
	{
		OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());			// �㲥����ֵ�ı�
		OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());		// �㲥�������ֵ�ı�
		OnManaChanged.Broadcast(AuraAttributeSet->GetMana());				// �㲥����ֵ�ı�
		OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());			// �㲥�����ֵ�ı�
	}
}
