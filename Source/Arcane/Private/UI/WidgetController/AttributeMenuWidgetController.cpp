// Copyright INVI1998


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues(const FGameplayTag& Tag)
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	checkf(AuraAttributeInfo, TEXT("AuraAttributeInfo is not set!"));	// ��Ҫ��֤��������ͼ�������������ʲ�

	for (const FAuraAttributeInfo& Info : AuraAttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Info.AttributeTag);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	checkf(AuraAttributeInfo, TEXT("AuraAttributeInfo is not set!"));	// ��Ҫ��֤��������ͼ�������������ʲ�

	for (const FAuraAttributeInfo& Tag : AuraAttributeInfo.Get()->AttributeInformation)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Tag.AttributeGetter).AddLambda(
			[this, Tag](const FOnAttributeChangeData& Data)->void
			{
				BroadcastAttributeInfo(Tag.AttributeTag);
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag)
{
	// �������ʲ�������Tagƥ���ȡ������Ϣ
	FAuraAttributeInfo Info = AuraAttributeInfo->GetAttributeInfo(Tag);

	// �����ݼ��е�ֵ��ֵ��Info
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);

	// �㲥������Ϣ
	AttributeInfoDelegate.Broadcast(Info);
}
