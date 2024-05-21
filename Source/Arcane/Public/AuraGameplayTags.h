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
	FGameplayTag Attributes_Secondary_Resistance_Physical;	// �������ԣ���������

	FGameplayTag Attributes_Meta_RewardExperience;			// Ԫ���ԣ���������

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

	FGameplayTag InputTag_Passive_1;							// ���룺����1
	FGameplayTag InputTag_Passive_2;							// ���룺����2

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
	FGameplayTag Abilities_HitReact;			// �������ܻ���Ӧ

	/*
	 * Fire Ability Tags
	 */
	FGameplayTag Abilities_Fire_FireBolt;				// ����: ���棺����
	FGameplayTag Abilities_Fire_FireBlast;				// ����: ���棺��
	FGameplayTag Abilities_Fire_FireNova;				// ����: ���棺��
	FGameplayTag Abilities_Fire_FireStorm;				// ����: ���棺��籩

	/*
	 * Ice Ability Tags
	 */
	FGameplayTag Abilities_Ice_Freeze;				// ����: ��˪������
	FGameplayTag Abilities_Ice_FrostBolt;			// ����: ��˪��˪����
	FGameplayTag Abilities_Ice_Blade;				// ����: ��˪������
	FGameplayTag Abilities_Ice_Blizzard;			// ����: ��˪������ѩ

	/*
	 * Lightning Ability Tags
	 */
	FGameplayTag Abilities_Lightning_Electrocute;	// ����: ���磺���
	FGameplayTag Abilities_Lightning_Thunder;		// ����: ���磺����
	FGameplayTag Abilities_Lightning_Chain;			// ����: ���磺������
	FGameplayTag Abilities_Lightning_Storm;			// ����: ���磺�ױ�

	/*
	 * Arcane Ability Tags
	 */
	FGameplayTag Abilities_Arcane_ArcaneMissile;	// ����: �����������ɵ�
	FGameplayTag Abilities_Arcane_Spellbind;		// ����: ��������������
	FGameplayTag Abilities_Arcane_Shield;			// ����: ����������
	FGameplayTag Abilities_Arcane_Lgnition;			// ����: ��������ȼ

	/*
	 * Poison Ability Tags
	 */
	FGameplayTag Abilities_Poison_PoisonDart;		// ����: ���أ�����
	FGameplayTag Abilities_Poison_Venom;			// ����: ���أ���Һ
	FGameplayTag Abilities_Poison_ToxicCloud;		// ����: ���أ�����
	FGameplayTag Abilities_Poison_PoisonNova;		// ����: ���أ�����

	/*
	 * Physical Ability Tags
	 */
	FGameplayTag Abilities_Physical_Slash;			// ����: ������ն��
	FGameplayTag Abilities_Physical_Stab;			// ����: �������̻�
	FGameplayTag Abilities_Physical_Swing;			// ����: �������ӻ�
	FGameplayTag Abilities_Physical_Thrust;			// ����: �������̵�

	/*
	 * Ability State Tags
	 */
	FGameplayTag Abilities_State_Locked;		// ����״̬������
	FGameplayTag Abilities_State_Eligible;		// ����״̬������
	FGameplayTag Abilities_State_UnLocked;		// ����״̬������
	FGameplayTag Abilities_State_Equipped;		// ����״̬��װ��

	/*
	 * Ability Type Tags
	 */
	FGameplayTag Abilities_Type_Offensive;		// �������ͣ�����
	FGameplayTag Abilities_Type_Passive;		// �������ͣ�����
	FGameplayTag Abilities_Type_None;			// �������ͣ���

	/*
	 * Cooldown Tags
	 */
	FGameplayTag Cooldown_Fire_FireBolt;		// ��ȴ�����棺����
	FGameplayTag Cooldown_Fire_FireBlast;		// ��ȴ�����棺��
	FGameplayTag Cooldown_Fire_FireNova;		// ��ȴ�����棺��
	FGameplayTag Cooldown_Fire_FireStorm;		// ��ȴ�����棺��籩
	FGameplayTag Cooldown_Ice_Freeze;			// ��ȴ����˪������
	FGameplayTag Cooldown_Ice_FrostBolt;		// ��ȴ����˪��˪����
	FGameplayTag Cooldown_Ice_Blade;			// ��ȴ����˪������
	FGameplayTag Cooldown_Ice_Blizzard;		// ��ȴ����˪������ѩ
	FGameplayTag Cooldown_Lightning_Electrocute;	// ��ȴ�����磺���
	FGameplayTag Cooldown_Lightning_Thunder;		// ��ȴ�����磺����
	FGameplayTag Cooldown_Lightning_Chain;		// ��ȴ�����磺������
	FGameplayTag Cooldown_Lightning_Storm;		// ��ȴ�����磺�ױ�
	FGameplayTag Cooldown_Arcane_ArcaneMissile;	// ��ȴ�������������ɵ�
	FGameplayTag Cooldown_Arcane_Spellbind;		// ��ȴ����������������
	FGameplayTag Cooldown_Arcane_Shield;			// ��ȴ������������
	FGameplayTag Cooldown_Arcane_Lgnition;		// ��ȴ����������ȼ
	FGameplayTag Cooldown_Poison_PoisonDart;		// ��ȴ�����أ�����
	FGameplayTag Cooldown_Poison_Venom;			// ��ȴ�����أ���Һ
	FGameplayTag Cooldown_Poison_ToxicCloud;		// ��ȴ�����أ�����
	FGameplayTag Cooldown_Poison_PoisonNova;		// ��ȴ�����أ�����
	FGameplayTag Cooldown_Physical_Slash;		// ��ȴ��������ն��
	FGameplayTag Cooldown_Physical_Stab;			// ��ȴ���������̻�
	FGameplayTag Cooldown_Physical_Swing;		// ��ȴ���������ӻ�
	FGameplayTag Cooldown_Physical_Thrust;		// ��ȴ���������̵�

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