// Copyright INVI1998


#include "AbilitySystem/AuraAttributeSet.h"

#include "Net/UnrealNetwork.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(100.f);	// ��ʼ������ֵ
	InitMaxHealth(100.f);	// ��ʼ���������ֵ
	InitMana(50.f);		// ��ʼ��ħ��
	InitMaxMana(50.f);	// ��ʼ�����ħ��
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);	// �������Եĸ��Ʒ�ʽ������������������������֪ͨ��ʽ
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);	// ֪ͨ���Ա仯����������������ֵ�;�ֵ
}
