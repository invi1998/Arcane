// Copyright INVI1998


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AttributeSet.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);	// ��AttributeSetת��ΪUAuraAttributeSet

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());			// �㲥����ֵ�ı�
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());		// �㲥�������ֵ�ı�
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());				// �㲥����ֵ�ı�
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());			// �㲥�����ֵ�ı�
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);	// ��AttributeSetת��ΪUAuraAttributeSet

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);	// �������ֵ�ı��ί��
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);	// ����������ֵ�ı��ί��
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);	// ��ӷ���ֵ�ı��ί��
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);	// ��������ֵ�ı��ί��
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);	// �㲥����ֵ�ı�
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);	// �㲥�������ֵ�ı�
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);	// �㲥����ֵ�ı�
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);	// �㲥�����ֵ�ı�
}
