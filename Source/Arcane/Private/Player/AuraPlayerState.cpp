// Copyright INVI1998


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.0f;		// ���ÿͻ���ÿ��100�θ��£�����˳��Ը��¿ͻ��˵�Ƶ��

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));	// ��������ϵͳ���
	AbilitySystemComponent->SetIsReplicated(true);	// ����Ϊ�ɸ���
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);	// ���ø���ģʽ�����ģʽ�����ڶ�����Ϸ��˵���ر�����ҿ��ƵĽ�ɫ������ΪMixed����õ�ѡ��

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));		// �������Լ�

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
