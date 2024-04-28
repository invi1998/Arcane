// Copyright INVI1998


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Fonts/UnicodeBlockRange.h"
#include "Kismet/KismetSystemLibrary.h"


void UOverlayWidgetController::BroadcastInitialValues(const FGameplayTag& Tag)
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);	// 将AttributeSet转换为UAuraAttributeSet

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());			// 广播生命值改变
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());		// 广播最大生命值改变
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());				// 广播法力值改变
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());			// 广播最大法力值改变
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);	// 将AttributeSet转换为UAuraAttributeSet

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda( 
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnHealthChanged.Broadcast(Data.NewValue);	// 广播生命值改变
		});	// 添加生命值改变的委托

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(	
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);	// 广播最大生命值改变
		});	// 添加最大生命值改变的委托)

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnManaChanged.Broadcast(Data.NewValue);	// 广播法力值改变
		});	// 添加法力值改变的委托)

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(		
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);	// 广播最大法力值改变
		});	// 添加最大法力值改变的委托)

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssertTags)
			{
				for (const FGameplayTag& Tag : AssertTags)
				{
					// 这里我们只要MessageTag
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));	// 获取Message
					if (Tag.MatchesTag(MessageTag))
					{
						// UKismetSystemLibrary::PrintString(GEngine->GetWorld(), Tag.ToString(), true, true, FLinearColor::Green, 5.0f);	// 打印Tag

						const FUIWidgetRow* WidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);	// 通过Tag获取数据表行
						// 我们希望通过Tag来获取数据表行，然后使用这些数据来更新UI，比如使用它里面的一些资产来显示内容

						MessageWidgetRowDelegate.Broadcast(*WidgetRow);	// 广播消息小部件行
					}
				}
			}
		);

		// 我们必须考虑到在初始化时，我们可能还没有给予能力，所以我们需要检查是否已经给予了能力
		if (AuraASC->bStartupAbilitiesGiven)
		{
			// 如果已经给予了能力，我们就可以直接调用OnInitializedStartupAbilities
			OnInitializedStartupAbilities(AuraASC);	// 初始化启动能力
		}
		else
		{
			// 如果没有给予能力，我们就需要添加一个委托，以便在给予能力时调用OnInitializedStartupAbilities
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializedStartupAbilities);
		}
	}

}

void UOverlayWidgetController::OnInitializedStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	// 获取所有给定Abilities的信息，查找他们的AbilityInfo并进行广播给Widget
	if (!AuraASC || !AuraASC->bStartupAbilitiesGiven) return;	// 如果AuraASC为空或者bStartupAbilitiesGiven为false，返回

	FForEachAbility BroadcastAbilityDelegate;
	BroadcastAbilityDelegate.BindLambda(
		[this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)->void
		{
			// 通过AbilityTag获取AbilityInfo
			const FGameplayTag tag = AuraASC->GetAbilityTagBySpec(AbilitySpec);
			FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(tag);	// 通过Tag查找AbilityInfo

			// 获取输入Tag
			const FGameplayTag inputTag = AuraASC->GetAbilityInputTagBySpec(AbilitySpec);
			AbilityInfo.InputTag = inputTag;	// 设置输入Tag

			// 广播委托
			AbilityInfoDelegate.Broadcast(AbilityInfo);	// 广播能力信息
		}
	);
	AuraASC->ForEachAbility(BroadcastAbilityDelegate);	// 对每个能力进行广播
}

