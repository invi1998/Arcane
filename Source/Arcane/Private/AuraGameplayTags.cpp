// Copyright INVI1998


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;	// 静态变量初始化

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// 添加原生标签，属性->次要属性->护甲，描述为减少受到的伤害，提高格挡几率
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage Taken, improves Block Chance"));
	// 添加原生标签，属性->次要属性->护甲，描述为减少受到的伤害，提高格挡几率
}
