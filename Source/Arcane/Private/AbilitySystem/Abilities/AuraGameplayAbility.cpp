// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s，</>\n<Default>等级：</><Level>%d</>"), L"技能名 - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s，</>\n<Default>等级：</><Level>%d</>"), L"技能名 - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level + 1);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s，</>\n<Default>当前技能已锁定，解锁等级：</><Level>%d</>"), L"技能名 - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

float UAuraGameplayAbility::GetManaCost(float Level)
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* GameplayEffect = GetCostGameplayEffect())
	{
		// 获取修饰器，然后遍历修饰器，获取法力消耗
		for (const FGameplayModifierInfo& ModifierInfo : GameplayEffect->Modifiers)
		{
			// 如果修饰器的属性是法力属性
			if (ModifierInfo.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				ManaCost = ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
				break;
			}
		}

	}

	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float Level)
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* GameplayEffect = GetCooldownGameplayEffect())
	{
		// 技能冷却直接通过修饰器获取（DurationMagnitude：持续时间）
		GameplayEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Cooldown);
	}

	return Cooldown;
}
