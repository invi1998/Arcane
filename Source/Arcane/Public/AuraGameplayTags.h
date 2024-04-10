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

	static void InitializeNativeGameplayTags();		// 初始化原生GameplayTags

	FGameplayTag Attributes_Secondary_Armor;			// 二级属性：护甲

protected:

private:
	static FAuraGameplayTags GameplayTags;
};