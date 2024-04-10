// Copyright INVI1998


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);

	check(AuraAttributeInfo);	// 确保AttributeInfo不为空

	// 获取属性信息
	FAuraAttributeInfo Info = AuraAttributeInfo->GetAttributeInfo(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AuraAttributeSet->GetStrength();	// 将属性值设置为当前值

	// 广播属性信息
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}
