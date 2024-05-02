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

	checkf(AuraAttributeInfo, TEXT("AuraAttributeInfo is not set!"));	// 需要保证我们在蓝图中设置了数据资产

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
	checkf(AuraAttributeInfo, TEXT("AuraAttributeInfo is not set!"));	// 需要保证我们在蓝图中设置了数据资产

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
	// 从数据资产中依据Tag匹配获取属性信息
	FAuraAttributeInfo Info = AuraAttributeInfo->GetAttributeInfo(Tag);

	// 将数据集中的值赋值给Info
	Info.AttributeValue = Info.AttributeGetter.GetNumericValue(AttributeSet);

	// 广播属性信息
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& Tag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(Tag);
}
