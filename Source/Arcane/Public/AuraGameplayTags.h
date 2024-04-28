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

	FGameplayTag Attributes_Secondary_Resistance_Fire;		// �������ԣ����濹��
	FGameplayTag Attributes_Secondary_Resistance_Ice;		// �������ԣ���˪����
	FGameplayTag Attributes_Secondary_Resistance_Lightning;	// �������ԣ����翹��
	FGameplayTag Attributes_Secondary_Resistance_Poison;	// �������ԣ����ؿ���
	FGameplayTag Attributes_Secondary_Resistance_Arcane;	// �������ԣ���������
	FGameplayTag Attributes_Secondary_Resistance_Physical;	// �������ԣ�������

	/*
	 * Input
	 */
	FGameplayTag InputTag_LeftMouseButton;						// ���룺���
	FGameplayTag InputTag_RightMouseButton;						// ���룺�Ҽ�
	FGameplayTag InputTag_1;									// ���룺1
	FGameplayTag InputTag_2;									// ���룺2
	FGameplayTag InputTag_3;									// ���룺3
	FGameplayTag InputTag_4;									// ���룺4
	FGameplayTag InputTag_5;									// ���룺5
	FGameplayTag InputTag_6;									// ���룺6
	FGameplayTag InputTag_7;									// ���룺7
	FGameplayTag InputTag_8;									// ���룺8
	FGameplayTag InputTag_9;									// ���룺9
	FGameplayTag InputTag_0;									// ���룺0

	/*
	 * Meta Tags
	 */
	FGameplayTag Damage;						// Ԫ��ǩ���ܵ��˺�

	FGameplayTag Damage_Fire;					// �����˺�
	FGameplayTag Damage_Ice;					// ��˪�˺�
	FGameplayTag Damage_Lightning;				// �����˺�
	FGameplayTag Damage_Poison;					// �����˺�
	FGameplayTag Damage_Arcane;					// �����˺�
	FGameplayTag Damage_Physical;				// �����˺�

	/*
	 * Ability Tags
	 */
	FGameplayTag Abilities_Attack;				// ����������
	FGameplayTag Abilities_Call;				// �������ٻ�
	FGameplayTag Abilities_Fire_FireBolt;				// ����: ���棺����

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;	// �˺����Ͷ�Ӧ����

	/*
	 * Effect Tags
	 */
	FGameplayTag Effect_HitReact;				// Ч�����ܻ���Ӧ
	FGameplayTag Effect_DeathReact;				// Ч����������Ӧ

	FGameplayTag Montage_Attack_Weapon;				// ��̫�棺��������
	FGameplayTag Montage_Attack_LeftWeapon;			// ��̫�棺������������
	FGameplayTag Montage_Attack_LeftWeaponLeftSwing;	// ��̫�棺�����������
	FGameplayTag Montage_Attack_LeftWeaponRightSwing;	// ��̫�棺���������һ�
	FGameplayTag Montage_Attack_RightWeapon;		// ��̫�棺������������
	FGameplayTag Montage_Attack_RightWeaponLeftSwing;	// ��̫�棺�����������
	FGameplayTag Montage_Attack_RightWeaponRightSwing;	// ��̫�棺���������һ�
	FGameplayTag Montage_Attack_Unarmed;			// ��̫�棺ͽ�ֹ���
	FGameplayTag Montage_Attack_LeftUnarmed;		// ��̫�棺����ͽ�ֹ���
	FGameplayTag Montage_Attack_RightUnarmed;		// ��̫�棺����ͽ�ֹ���
	FGameplayTag Montage_Attack_Bow;				// ��̫�棺��������
	FGameplayTag Montage_Attack_LichCall;			// ��̫�棺�����ٻ�
	FGameplayTag Montage_Attack_LichSpell;			// ��̫�棺��������
	


protected:

private:
	static FAuraGameplayTags GameplayTags;
};