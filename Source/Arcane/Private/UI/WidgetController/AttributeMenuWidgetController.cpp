// Copyright INVI1998


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues(const FGameplayTag& Tag)
{
	// UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	checkf(AuraAttributeInfo, TEXT("AuraAttributeInfo is not set!"));	// ��Ҫ��֤��������ͼ�������������ʲ�

	for (const FAuraAttributeInfo& Info : AuraAttributeInfo.Get()->AttributeInformation)
	{
		BroadcastAttributeInfo(Info.AttributeTag);
	}

	AAuraPlayerState* PS = CastChecked<AAuraPlayerState>(PlayerState);
	AttributePointChangeDelegate.Broadcast(PS->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
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

	AAuraPlayerState* PS = CastChecked<AAuraPlayerState>(PlayerState);
	PS->OnAttributePointChangedDelegate.AddLambda(
		[this](int32 Points)->void
		{
			AttributePointChangeDelegate.Broadcast(Points);
		}
	);
	
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

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& Tag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(Tag);
}
