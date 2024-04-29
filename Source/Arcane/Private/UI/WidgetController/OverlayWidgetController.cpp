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
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);	// ��AttributeSetת��ΪUAuraAttributeSet

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());			// �㲥����ֵ�ı�
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());		// �㲥�������ֵ�ı�
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());				// �㲥����ֵ�ı�
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());			// �㲥�����ֵ�ı�
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(this->AttributeSet);	// ��AttributeSetת��ΪUAuraAttributeSet
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(this->PlayerState);	// ��PlayerStateת��ΪAAuraPlayerState

	AuraPlayerState->OnExpChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnExpChanged);	// ��Ӿ���ı��ί��

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda( 
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnHealthChanged.Broadcast(Data.NewValue);	// �㲥����ֵ�ı�
		});	// �������ֵ�ı��ί��

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(	
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);	// �㲥�������ֵ�ı�
		});	// ����������ֵ�ı��ί��)

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnManaChanged.Broadcast(Data.NewValue);	// �㲥����ֵ�ı�
		});	// ��ӷ���ֵ�ı��ί��)

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(		
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);	// �㲥�����ֵ�ı�
		});	// ��������ֵ�ı��ί��)

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		AuraASC->EffectAssetTags.AddLambda(
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
		if (AuraASC->bStartupAbilitiesGiven)
		{
			// ����Ѿ����������������ǾͿ���ֱ�ӵ���OnInitializedStartupAbilities
			OnInitializedStartupAbilities(AuraASC);	// ��ʼ����������
		}
		else
		{
			// ���û�и������������Ǿ���Ҫ���һ��ί�У��Ա��ڸ�������ʱ����OnInitializedStartupAbilities
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializedStartupAbilities);
		}
	}

}

void UOverlayWidgetController::OnInitializedStartupAbilities(UAuraAbilitySystemComponent* AuraASC)
{
	// ��ȡ���и���Abilities����Ϣ���������ǵ�AbilityInfo�����й㲥��Widget
	if (!AuraASC || !AuraASC->bStartupAbilitiesGiven) return;	// ���AuraASCΪ�ջ���bStartupAbilitiesGivenΪfalse������

	FForEachAbility BroadcastAbilityDelegate;
	BroadcastAbilityDelegate.BindLambda(
		[this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)->void
		{
			// ͨ��AbilityTag��ȡAbilityInfo
			const FGameplayTag tag = AuraASC->GetAbilityTagBySpec(AbilitySpec);
			FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(tag);	// ͨ��Tag����AbilityInfo

			// ��ȡ����Tag
			const FGameplayTag inputTag = AuraASC->GetAbilityInputTagBySpec(AbilitySpec);
			AbilityInfo.InputTag = inputTag;	// ��������Tag

			// �㲥ί��
			AbilityInfoDelegate.Broadcast(AbilityInfo);	// �㲥������Ϣ
		}
	);
	AuraASC->ForEachAbility(BroadcastAbilityDelegate);	// ��ÿ���������й㲥
}

void UOverlayWidgetController::OnExpChanged(int32 NewExp) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(this->PlayerState);	// ��PlayerStateת��ΪAAuraPlayerState

	checkf(AuraPlayerState->LevelUpInfo, TEXT("LevelUpInfo is not set in AuraPlayerState"));	// ���LevelUpInfo�Ƿ�Ϊ��

	// ��ȡ��ҵȼ�
	int32 PlayerLevel = AuraPlayerState->LevelUpInfo->GetLevelByExp(NewExp);	// ��ȡ��ҵȼ�
	PlayerLevel = FMath::Clamp(PlayerLevel, 1, AuraPlayerState->LevelUpInfo->LevelUpInformation.Num());	// ������ҵȼ�

	// ���������ҵ��ܾ��飬������Ҫ�õ���ǰ�ȼ��ľ���
	const int32 CurrentLevelExp = AuraPlayerState->LevelUpInfo->GetCurrentLevelTotalExp(PlayerLevel);	// ��ȡ��ǰ�ȼ����ܾ���

	const int32 NextLevelExp = AuraPlayerState->LevelUpInfo->GetLevelUpRequiredExp(PlayerLevel + 1);	// ��ȡ��һ�����ܾ���

	const float ExpPercent = static_cast<float>(NewExp - CurrentLevelExp) / static_cast<float>(NextLevelExp);	// ���㾭��ٷֱ�

	OnExpPercentChangedDelegate.Broadcast(ExpPercent);	// �㲥����ٷֱ�
}

