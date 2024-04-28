// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * 原生单例，用于管理游戏中的所有GameplayTags
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; };

	static void InitializeNativeGameplayTags();				// 初始化原生GameplayTags

	/*
	 * 主属性（Primary Attributes）
	 */
	FGameplayTag Attributes_Primary_Strength;				// 主属性：力量
	FGameplayTag Attributes_Primary_Agility;				// 主属性：敏捷
	FGameplayTag Attributes_Primary_Intelligence;			// 主属性：智力
	FGameplayTag Attributes_Primary_Vigor;					// 主属性：活力
	FGameplayTag Attributes_Primary_Resilience;				// 主属性：坚韧

	/*
	 *	次要属性（Secondary Attributes） 或者说是派生属性（Derived Attributes）
	 */

	FGameplayTag Attributes_Secondary_MaxHealth;			// 二级属性：最大生命值
	FGameplayTag Attributes_Secondary_MaxMana;				// 二级属性：最大法力值

	FGameplayTag Attributes_Secondary_Armor;				// 二级属性：护甲
	FGameplayTag Attributes_Secondary_ArmorPenetration;		// 二级属性：护甲穿透
	FGameplayTag Attributes_Secondary_BlockChance;			// 二级属性：格挡几率
	FGameplayTag Attributes_Secondary_CriticalHitChance;	// 二级属性：暴击几率
	FGameplayTag Attributes_Secondary_CriticalHitDamage;	// 二级属性: 暴击伤害
	FGameplayTag Attributes_Secondary_CriticalHitResistance;	// 二级属性: 暴击抗性
	FGameplayTag Attributes_Secondary_HealthRegeneration;	// 二级属性: 生命回复
	FGameplayTag Attributes_Secondary_ManaRegeneration;		// 二级属性: 法力回复

	FGameplayTag Attributes_Secondary_Resistance_Fire;		// 二级属性：火焰抗性
	FGameplayTag Attributes_Secondary_Resistance_Ice;		// 二级属性：冰霜抗性
	FGameplayTag Attributes_Secondary_Resistance_Lightning;	// 二级属性：闪电抗性
	FGameplayTag Attributes_Secondary_Resistance_Poison;	// 二级属性：毒素抗性
	FGameplayTag Attributes_Secondary_Resistance_Arcane;	// 二级属性：奥术抗性
	FGameplayTag Attributes_Secondary_Resistance_Physical;	// 二级属性：物理抗性

	/*
	 * Input
	 */
	FGameplayTag InputTag_LeftMouseButton;						// 输入：左键
	FGameplayTag InputTag_RightMouseButton;						// 输入：右键
	FGameplayTag InputTag_1;									// 输入：1
	FGameplayTag InputTag_2;									// 输入：2
	FGameplayTag InputTag_3;									// 输入：3
	FGameplayTag InputTag_4;									// 输入：4
	FGameplayTag InputTag_5;									// 输入：5
	FGameplayTag InputTag_6;									// 输入：6
	FGameplayTag InputTag_7;									// 输入：7
	FGameplayTag InputTag_8;									// 输入：8
	FGameplayTag InputTag_9;									// 输入：9
	FGameplayTag InputTag_0;									// 输入：0

	/*
	 * Meta Tags
	 */
	FGameplayTag Damage;						// 元标签：受到伤害

	FGameplayTag Damage_Fire;					// 火焰伤害
	FGameplayTag Damage_Ice;					// 冰霜伤害
	FGameplayTag Damage_Lightning;				// 闪电伤害
	FGameplayTag Damage_Poison;					// 毒素伤害
	FGameplayTag Damage_Arcane;					// 奥术伤害
	FGameplayTag Damage_Physical;				// 物理伤害

	/*
	 * Ability Tags
	 */
	FGameplayTag Abilities_Attack;				// 能力：攻击
	FGameplayTag Abilities_Call;				// 能力：召唤
	FGameplayTag Abilities_Fire_FireBolt;				// 能力: 火焰：火球

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;	// 伤害类型对应抗性

	/*
	 * Effect Tags
	 */
	FGameplayTag Effect_HitReact;				// 效果：受击反应
	FGameplayTag Effect_DeathReact;				// 效果：死亡反应

	FGameplayTag Montage_Attack_Weapon;				// 蒙太奇：武器攻击
	FGameplayTag Montage_Attack_LeftWeapon;			// 蒙太奇：左手武器攻击
	FGameplayTag Montage_Attack_LeftWeaponLeftSwing;	// 蒙太奇：左手武器左挥
	FGameplayTag Montage_Attack_LeftWeaponRightSwing;	// 蒙太奇：左手武器右挥
	FGameplayTag Montage_Attack_RightWeapon;		// 蒙太奇：右手武器攻击
	FGameplayTag Montage_Attack_RightWeaponLeftSwing;	// 蒙太奇：右手武器左挥
	FGameplayTag Montage_Attack_RightWeaponRightSwing;	// 蒙太奇：右手武器右挥
	FGameplayTag Montage_Attack_Unarmed;			// 蒙太奇：徒手攻击
	FGameplayTag Montage_Attack_LeftUnarmed;		// 蒙太奇：左手徒手攻击
	FGameplayTag Montage_Attack_RightUnarmed;		// 蒙太奇：右手徒手攻击
	FGameplayTag Montage_Attack_Bow;				// 蒙太奇：弓箭攻击
	FGameplayTag Montage_Attack_LichCall;			// 蒙太奇：巫妖召唤
	FGameplayTag Montage_Attack_LichSpell;			// 蒙太奇：巫妖法术
	


protected:

private:
	static FAuraGameplayTags GameplayTags;
};