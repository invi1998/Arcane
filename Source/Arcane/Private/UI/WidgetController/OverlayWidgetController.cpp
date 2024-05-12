// Copyright INVI1998


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Fonts/UnicodeBlockRange.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/AuraPlayerState.h"


void UOverlayWidgetController::BroadcastInitialValues(const FGameplayTag& Tag)
{
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());			// �㲥����ֵ�ı�
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());		// �㲥�������ֵ�ı�
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());				// �㲥����ֵ�ı�
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());			// �㲥�����ֵ�ı�
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnExpChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnExpChanged);	// ��Ӿ���ı��ί��
	GetAuraPS()->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnPlayerLevelChanged);	// ��ӵȼ��ı��ί��
	GetAuraPS()->OnAttributePointChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnAttributePointsChanged);	// ������Ե�ı��ί��
	GetAuraPS()->OnSkillPointChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnSkillPointsChanged);	// ��Ӽ��ܵ�ı��ί��

	GetAuraASC()->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddLambda( 
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnHealthChanged.Broadcast(Data.NewValue);	// �㲥����ֵ�ı�
		});	// �������ֵ�ı��ί��

	GetAuraASC()->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);	// �㲥�������ֵ�ı�
		});	// ����������ֵ�ı��ί��)

	GetAuraASC()->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnManaChanged.Broadcast(Data.NewValue);	// �㲥����ֵ�ı�
		});	// ��ӷ���ֵ�ı��ί��)

	GetAuraASC()->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);	// �㲥�����ֵ�ı�
		});	// ��������ֵ�ı��ί��)

	if (GetAuraASC())
	{
		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssertTags)
			{
				for (const FGameplayTag& Tag : AssertTags)
				{
					// ��������ֻҪMessageTag
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));	// ��ȡMessage
					if (Tag.MatchesTag(MessageTag))
					{
						// UKismetSystemLibrary::PrintString(GEngine->GetWorld(), Tag.ToString(), true, true, FLinearColor::Green, 5.0f);	// ��ӡTag

						const FUIWidgetRow* WidgetRow = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);	// ͨ��Tag��ȡ���ݱ���
						// ����ϣ��ͨ��Tag����ȡ���ݱ��У�Ȼ��ʹ����Щ����������UI������ʹ���������һЩ�ʲ�����ʾ����

						MessageWidgetRowDelegate.Broadcast(*WidgetRow);	// �㲥��ϢС������
					}
				}
			}
		);

		// ���Ǳ��뿼�ǵ��ڳ�ʼ��ʱ�����ǿ��ܻ�û�и�������������������Ҫ����Ƿ��Ѿ�����������
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			// ����Ѿ����������������ǾͿ���ֱ�ӵ���OnInitializedStartupAbilities
			BroadcastAbilityInfo();	// �㲥������Ϣ
		}
		else
		{
			// ���û�и������������Ǿ���Ҫ���һ��ί�У��Ա��ڸ�������ʱ����OnInitializedStartupAbilities
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
	}

}

void UOverlayWidgetController::OnExpChanged(int32 NewExp)
{

	checkf(GetAuraPS()->LevelUpInfo, TEXT("LevelUpInfo is not set in AuraPlayerState"));	// ���LevelUpInfo�Ƿ�Ϊ��

	// ��ȡ��ҵȼ�
	int32 PlayerLevel = GetAuraPS()->LevelUpInfo->GetLevelByExp(NewExp);	// ��ȡ��ҵȼ�
	PlayerLevel = FMath::Clamp(PlayerLevel, 1, GetAuraPS()->LevelUpInfo->LevelUpInformation.Num());	// ������ҵȼ�

	// ���������ҵ��ܾ��飬������Ҫ�õ���ǰ�ȼ��ľ���
	const int32 CurrentLevelExp = GetAuraPS()->LevelUpInfo->GetCurrentLevelTotalExp(PlayerLevel);	// ��ȡ��ǰ�ȼ����ܾ���

	const int32 NextLevelExp = GetAuraPS()->LevelUpInfo->GetLevelUpRequiredExp(PlayerLevel + 1);	// ��ȡ��һ�����ܾ���

	const float ExpPercent = static_cast<float>(NewExp - CurrentLevelExp) / static_cast<float>(NextLevelExp);	// ���㾭��ٷֱ�

	OnExpPercentChangedDelegate.Broadcast(ExpPercent);	// �㲥����ٷֱ�
}

void UOverlayWidgetController::OnPlayerLevelChanged(int32 NewLevel) const
{
	OnLevelChangedDelegate.Broadcast(NewLevel);	// �㲥�ȼ��ı�
}

void UOverlayWidgetController::OnAttributePointsChanged(int32 NewAttributePoints) const
{
	OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints);	// �㲥���Ե�ı�
}

void UOverlayWidgetController::OnSkillPointsChanged(int32 NewSkillPoints) const
{
	OnSkillPointsChangedDelegate.Broadcast(NewSkillPoints);	// �㲥���ܵ�ı�
}

