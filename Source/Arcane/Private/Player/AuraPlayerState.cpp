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
	const int32 MatchedLevel = LevelUpInfo->GetLevelByExp(EXP);
	if (MatchedLevel > Level)
	{
		AddLevel(MatchedLevel - Level);
	}
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

	if (GetOwner()->Implements<UPlayerInterface>())
	{
		// ��ȡ���������Ե���
		const int32 AttributePoints = LevelUpInfo->GetAttributePointRewardByLevel(Level);
		if (AttributePoints > 0)
		{
			// ������Ե�
			IPlayerInterface::Execute_AddAttributePoint(GetOwner(), AttributePoints);
		}
		// ��ȡ�����ļ��ܵ���
		const int32 SkillPoints = LevelUpInfo->GetSkillPointRewardByLevel(Level);
		if (SkillPoints > 0)
		{
			// ��Ӽ��ܵ�
			IPlayerInterface::Execute_AddSkillPoint(GetOwner(), SkillPoints);
		}
	}

}

void AAuraPlayerState::SetLevel(int32 Value)
{
	Level = Value;
	OnLevelChangedDelegate.Broadcast(Level);

	if (GetOwner()->Implements<UPlayerInterface>())
	{
		// ��ȡ���������Ե���
		const int32 AttributePoints = LevelUpInfo->GetTotalAttributePointRewardInLevel(Level);
		if (AttributePoints > 0)
		{
			// ������Ե�
			IPlayerInterface::Execute_AddAttributePoint(GetOwner(), AttributePoints);
		}
		// ��ȡ�����ļ��ܵ���
		const int32 SkillPoints = LevelUpInfo->GetTotalSkillPointRewardInLevel(Level);
		if (SkillPoints > 0)
		{
			// ��Ӽ��ܵ�
			IPlayerInterface::Execute_AddSkillPoint(GetOwner(), SkillPoints);
		}
	}
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
