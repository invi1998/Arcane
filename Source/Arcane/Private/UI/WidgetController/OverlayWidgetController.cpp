// Copyright INVI1998


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Fonts/UnicodeBlockRange.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Player/AuraPlayerState.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());			// 广播生命值改变
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());		// 广播最大生命值改变
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());				// 广播法力值改变
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());			// 广播最大法力值改变
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetAuraPS()->OnExpChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnExpChanged);	// 添加经验改变的委托
	GetAuraPS()->OnLevelChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnPlayerLevelChanged);	// 添加等级改变的委托
	GetAuraPS()->OnAttributePointChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnAttributePointsChanged);	// 添加属性点改变的委托
	GetAuraPS()->OnSkillPointChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnSkillPointsChanged);	// 添加技能点改变的委托

	GetAuraASC()->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddLambda( 
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnHealthChanged.Broadcast(Data.NewValue);	// 广播生命值改变
		});	// 添加生命值改变的委托

	GetAuraASC()->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);	// 广播最大生命值改变
		});	// 添加最大生命值改变的委托)

	GetAuraASC()->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnManaChanged.Broadcast(Data.NewValue);	// 广播法力值改变
		});	// 添加法力值改变的委托)

	GetAuraASC()->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)->void
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);	// 广播最大法力值改变
		});	// 添加最大法力值改变的委托)

	if (GetAuraASC())
	{
		GetAuraASC()->AbilitySlotChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnAbilitySlotChange);	// 添加技能槽改变的委托

		GetAuraASC()->EffectAssetTags.AddLambda(
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
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			// 如果已经给予了能力，我们就可以直接调用OnInitializedStartupAbilities
			BroadcastAbilityInfo();	// 广播能力信息
		}
		else
		{
			// 如果没有给予能力，我们就需要添加一个委托，以便在给予能力时调用OnInitializedStartupAbilities
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}
	}

}

void UOverlayWidgetController::OnExpChanged(int32 NewExp)
{

	checkf(GetAuraPS()->LevelUpInfo, TEXT("LevelUpInfo is not set in AuraPlayerState"));	// 检查LevelUpInfo是否为空

	// 获取玩家等级
	int32 PlayerLevel = GetAuraPS()->LevelUpInfo->GetLevelByExp(NewExp);	// 获取玩家等级
	PlayerLevel = FMath::Clamp(PlayerLevel, 1, GetAuraPS()->LevelUpInfo->LevelUpInformation.Num());	// 限制玩家等级

	// 传入的是玩家的总经验，我们需要得到当前等级的经验
	const int32 CurrentLevelExp = GetAuraPS()->LevelUpInfo->GetCurrentLevelTotalExp(PlayerLevel);	// 获取当前等级的总经验

	const int32 NextLevelExp = GetAuraPS()->LevelUpInfo->GetLevelUpRequiredExp(PlayerLevel + 1);	// 获取下一级的总经验

	const float ExpPercent = static_cast<float>(NewExp - CurrentLevelExp) / static_cast<float>(NextLevelExp);	// 计算经验百分比

	OnExpPercentChangedDelegate.Broadcast(ExpPercent);	// 广播经验百分比
}

void UOverlayWidgetController::OnPlayerLevelChanged(int32 NewLevel) const
{
	OnLevelChangedDelegate.Broadcast(NewLevel);	// 广播等级改变
}

void UOverlayWidgetController::OnAttributePointsChanged(int32 NewAttributePoints) const
{
	OnAttributePointsChangedDelegate.Broadcast(NewAttributePoints);	// 广播属性点改变
}

void UOverlayWidgetController::OnSkillPointsChanged(int32 NewSkillPoints) const
{
	OnSkillPointsChangedDelegate.Broadcast(NewSkillPoints);	// 广播技能点改变
}

void UOverlayWidgetController::OnAbilitySlotChange(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& OldSlotTag) const
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	FAuraAbilityInfo LastSlotInfo;	// 上一个槽信息
	LastSlotInfo.StateTag = AuraTags.Abilities_State_UnLocked;	// 设置技能状态标签为解锁状态
	LastSlotInfo.InputTag = OldSlotTag;	// 设置输入标签为旧槽标签
	LastSlotInfo.AbilityTag = AuraTags.Abilities_None;	// 设置技能标签为空标签
	AbilityInfoDelegate.Broadcast(LastSlotInfo);	// 广播技能信息

	FAuraAbilityInfo AbilityInfo = AbilityInformation->FindAbilityInfoByTag(AbilityTag);	// 通过技能标签查找技能信息
	AbilityInfo.StateTag = StatusTag;	// 设置技能状态标签
	AbilityInfo.InputTag = SlotTag;	// 设置输入标签
	AbilityInfoDelegate.Broadcast(AbilityInfo);	// 广播技能信息
}

