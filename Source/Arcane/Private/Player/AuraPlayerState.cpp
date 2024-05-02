// Copyright INVI1998


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.0f;		// 设置客户端每秒100次更新，服务端尝试更新客户端的频率

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));	// 创建能力系统组件
	AbilitySystemComponent->SetIsReplicated(true);	// 设置为可复制
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);	// 设置复制模式，混合模式，对于多人游戏来说，特别是玩家控制的角色，设置为Mixed是最好的选择

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));		// 创建属性集

}

void AAuraPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, EXP);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AAuraPlayerState::AddEXP(int32 Value)
{
	EXP += Value;
	OnExpChangedDelegate.Broadcast(EXP);
}

void AAuraPlayerState::SetEXP(int32 Value)
{
	EXP = Value;
	const int32 MatchedLevel = LevelUpInfo->GetLevelByExp(EXP);
	if (MatchedLevel != Level)
	{
		SetLevel(MatchedLevel);
	}
	OnExpChangedDelegate.Broadcast(EXP);
}

void AAuraPlayerState::AddLevel(int32 Value)
{
	Level += Value;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetLevel(int32 Value)
{
	Level = Value;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddAttributePoint(int32 Value)
{
	AttributePoints += Value;
	OnAttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetAttributePoint(int32 Value)
{
	AttributePoints = Value;
	OnAttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddSkillPoint(int32 Value)
{
	SkillPoints += Value;
	OnSkillPointChangedDelegate.Broadcast(SkillPoints);
}

void AAuraPlayerState::SetSkillPoint(int32 Value)
{
	SkillPoints = Value;
	OnSkillPointChangedDelegate.Broadcast(SkillPoints);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_EXP(int32 OldEXP)
{
	OnExpChangedDelegate.Broadcast(EXP);
}

void AAuraPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	OnAttributePointChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SkillPoint(int32 OldSkillPoint)
{
	OnSkillPointChangedDelegate.Broadcast(SkillPoints);
}
