// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * ԭ�����������ڹ�����Ϸ�е�����GameplayTags
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; };

	static void InitializeNativeGameplayTags();				// ��ʼ��ԭ��GameplayTags

	/*
	 * �����ԣ�Primary Attributes��
	 */
	FGameplayTag Attributes_Primary_Strength;				// �����ԣ�����
	FGameplayTag Attributes_Primary_Agility;				// �����ԣ�����
	FGameplayTag Attributes_Primary_Intelligence;			// �����ԣ�����
	FGameplayTag Attributes_Primary_Vigor;					// �����ԣ�����
	FGameplayTag Attributes_Primary_Resilience;				// �����ԣ�����

	/*
	 *	��Ҫ���ԣ�Secondary Attributes�� ����˵���������ԣ�Derived Attributes��
	 */

	FGameplayTag Attributes_Secondary_MaxHealth;			// �������ԣ��������ֵ
	FGameplayTag Attributes_Secondary_MaxMana;				// �������ԣ������ֵ

	FGameplayTag Attributes_Secondary_Armor;				// �������ԣ�����
	FGameplayTag Attributes_Secondary_ArmorPenetration;		// �������ԣ����״�͸
	FGameplayTag Attributes_Secondary_BlockChance;			// �������ԣ��񵲼���
	FGameplayTag Attributes_Secondary_CriticalHitChance;	// �������ԣ���������
	FGameplayTag Attributes_Secondary_CriticalHitDamage;	// ��������: �����˺�
	FGameplayTag Attributes_Secondary_CriticalHitResistance;	// ��������: ��������
	FGameplayTag Attributes_Secondary_HealthRegeneration;	// ��������: �����ظ�
	FGameplayTag Attributes_Secondary_ManaRegeneration;		// ��������: �����ظ�


protected:

private:
	static FAuraGameplayTags GameplayTags;
};