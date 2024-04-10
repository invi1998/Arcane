// Copyright INVI1998


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	check(AuraAttributeInfo);	// ȷ��AttributeInfo��Ϊ��

	// ��ȡ������Ϣ
	FAuraAttributeInfo Info = AuraAttributeInfo->GetAttributeInfo(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AuraAttributeSet->GetStrength();	// ������ֵ����Ϊ��ǰֵ

	// �㲥������Ϣ
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}
