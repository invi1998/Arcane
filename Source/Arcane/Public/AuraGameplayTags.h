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


protected:

private:
	static FAuraGameplayTags GameplayTags;
};