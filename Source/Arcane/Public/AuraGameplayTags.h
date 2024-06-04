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
	static FString GetDamageCnName(const FGameplayTag& Tag);

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

	FGameplayTag Attributes_Meta_RewardExperience;			// 元属性：奖励经验

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

	FGameplayTag InputTag_Passive_1;							// 输入：被动1
	FGameplayTag InputTag_Passive_2;							// 输入：被动2

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
	 * DamageType Debuff Tags	每种伤害类型对应的Debuff
	 */
	FGameplayTag Debuff_FireBurn;				// 火焰灼烧
	FGameplayTag Debuff_IceFreeze;				// 冰霜冻结
	FGameplayTag Debuff_LightningStun;			// 闪电眩晕
	FGameplayTag Debuff_PoisonVenom;			// 毒素中毒
	FGameplayTag Debuff_ArcaneCurse;			// 奥术诅咒
	FGameplayTag Debuff_PhysicalBleed;			// 物理流血

	/*
	 * Ability Tags
	 */
	FGameplayTag Abilities_Attack;				// 能力：攻击
	FGameplayTag Abilities_Call;				// 能力：召唤
	FGameplayTag Abilities_HitReact;			// 能力：受击反应

	FGameplayTag Abilities_None;				// 能力：无

	/*
	 * Fire Ability Tags
	 */
	FGameplayTag Abilities_Fire_FireBolt;				// 能力: 火焰：火球
	FGameplayTag Abilities_Fire_FireBlast;				// 能力: 火焰：火爆
	FGameplayTag Abilities_Fire_FireNova;				// 能力: 火焰：火新星
	FGameplayTag Abilities_Fire_FireStorm;				// 能力: 火焰：火风暴

	/*
	 * Ice Ability Tags
	 */
	FGameplayTag Abilities_Ice_Freeze;				// 能力: 冰霜：冰冻
	FGameplayTag Abilities_Ice_FrostBolt;			// 能力: 冰霜：霜冰箭
	FGameplayTag Abilities_Ice_Blade;				// 能力: 冰霜：冰刃
	FGameplayTag Abilities_Ice_Blizzard;			// 能力: 冰霜：暴风雪

	/*
	 * Lightning Ability Tags
	 */
	FGameplayTag Abilities_Lightning_Electrocute;	// 能力: 闪电：电击
	FGameplayTag Abilities_Lightning_Thunder;		// 能力: 闪电：雷霆
	FGameplayTag Abilities_Lightning_Chain;			// 能力: 闪电：闪电链
	FGameplayTag Abilities_Lightning_Storm;			// 能力: 闪电：雷暴

	/*
	 * Arcane Ability Tags
	 */
	FGameplayTag Abilities_Arcane_ArcaneMissile;	// 能力: 奥术：奥术飞弹
	FGameplayTag Abilities_Arcane_Spellbind;		// 能力: 奥术：法术束缚
	FGameplayTag Abilities_Arcane_Shield;			// 能力: 奥术：护盾
	FGameplayTag Abilities_Arcane_Lgnition;			// 能力: 奥术：引燃

	/*
	 * Poison Ability Tags
	 */
	FGameplayTag Abilities_Poison_PoisonDart;		// 能力: 毒素：毒镖
	FGameplayTag Abilities_Poison_Venom;			// 能力: 毒素：毒液
	FGameplayTag Abilities_Poison_ToxicCloud;		// 能力: 毒素：毒云
	FGameplayTag Abilities_Poison_PoisonNova;		// 能力: 毒素：毒爆

	/*
	 * Physical Ability Tags
	 */
	FGameplayTag Abilities_Physical_Slash;			// 能力: 物理：斩击
	FGameplayTag Abilities_Physical_Stab;			// 能力: 物理：刺击
	FGameplayTag Abilities_Physical_Swing;			// 能力: 物理：挥击
	FGameplayTag Abilities_Physical_Thrust;			// 能力: 物理：刺刀

	/*
	 * Ability State Tags
	 */
	FGameplayTag Abilities_State_Locked;		// 能力状态：锁定
	FGameplayTag Abilities_State_Eligible;		// 能力状态：可用
	FGameplayTag Abilities_State_UnLocked;		// 能力状态：解锁
	FGameplayTag Abilities_State_Equipped;		// 能力状态：装备

	/*
	 * Ability Type Tags
	 */
	FGameplayTag Abilities_Type_Offensive;		// 能力类型：攻击
	FGameplayTag Abilities_Type_Passive;		// 能力类型：被动
	FGameplayTag Abilities_Type_None;			// 能力类型：无

	/*
	 * Cooldown Tags
	 */
	FGameplayTag Cooldown_Fire_FireBolt;		// 冷却：火焰：火球
	FGameplayTag Cooldown_Fire_FireBlast;		// 冷却：火焰：火爆
	FGameplayTag Cooldown_Fire_FireNova;		// 冷却：火焰：火爆
	FGameplayTag Cooldown_Fire_FireStorm;		// 冷却：火焰：火风暴
	FGameplayTag Cooldown_Ice_Freeze;			// 冷却：冰霜：冰冻
	FGameplayTag Cooldown_Ice_FrostBolt;		// 冷却：冰霜：霜冰箭
	FGameplayTag Cooldown_Ice_Blade;			// 冷却：冰霜：冰刃
	FGameplayTag Cooldown_Ice_Blizzard;		// 冷却：冰霜：暴风雪
	FGameplayTag Cooldown_Lightning_Electrocute;	// 冷却：闪电：电击
	FGameplayTag Cooldown_Lightning_Thunder;		// 冷却：闪电：雷霆
	FGameplayTag Cooldown_Lightning_Chain;		// 冷却：闪电：闪电链
	FGameplayTag Cooldown_Lightning_Storm;		// 冷却：闪电：雷暴
	FGameplayTag Cooldown_Arcane_ArcaneMissile;	// 冷却：奥术：奥术飞弹
	FGameplayTag Cooldown_Arcane_Spellbind;		// 冷却：奥术：法术束缚
	FGameplayTag Cooldown_Arcane_Shield;			// 冷却：奥术：护盾
	FGameplayTag Cooldown_Arcane_Lgnition;		// 冷却：奥术：引燃
	FGameplayTag Cooldown_Poison_PoisonDart;		// 冷却：毒素：毒镖
	FGameplayTag Cooldown_Poison_Venom;			// 冷却：毒素：毒液
	FGameplayTag Cooldown_Poison_ToxicCloud;		// 冷却：毒素：毒云
	FGameplayTag Cooldown_Poison_PoisonNova;		// 冷却：毒素：毒爆
	FGameplayTag Cooldown_Physical_Slash;		// 冷却：物理：斩击
	FGameplayTag Cooldown_Physical_Stab;			// 冷却：物理：刺击
	FGameplayTag Cooldown_Physical_Swing;		// 冷却：物理：挥击
	FGameplayTag Cooldown_Physical_Thrust;		// 冷却：物理：刺刀

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;	// 伤害类型对应抗性
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuff;		// 伤害类型对应Debuff

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

