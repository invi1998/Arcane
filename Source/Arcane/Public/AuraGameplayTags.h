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

	static void InitializeNativeGameplayTags();		// ��ʼ��ԭ��GameplayTags

	FGameplayTag Attributes_Secondary_Armor;			// �������ԣ�����

protected:

private:
	static FAuraGameplayTags GameplayTags;
};