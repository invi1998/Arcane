// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	// 更具Tag获取法力消耗
	const float Cooldown = GetCooldown(Level);
	const float ManaCost = GetManaCost(Level);

	// 遍历伤害类型，获取每种类型的伤害值，然后拼接字符串
	FString DamageTypeString;
	for (auto& DamagePair : DamageType)
	{
		const FGameplayTag& DamageTag = DamagePair.Key;
		const FScalableFloat& DamageValue = DamagePair.Value;
		const float ScaledDamageValue = DamageValue.GetValueAtLevel(Level);
		const FString TagName = FAuraGameplayTags::GetDamageCnName(DamageTag);
		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.2f</>\n"), *TagName, ScaledDamageValue);
	}

	FString Desc = FString::Printf(TEXT("<Title>火球术</>\t<Small>FIRE BOLT</>\n\n"
		"<Default>发射一道火焰，在撞击和造成伤害时爆炸 </>\n\n"
		"\t<Default>技能等级：</><Level>%d</>\n"
		"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
		"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
		"\t<Default>火球数量：</><Time>%d</>\n\n"
		"<Default>技能详细伤害描述：</>\n"
	), Level, Cooldown, ManaCost, NumProjectiles);
	Desc += DamageTypeString;

	return Desc;
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	// 更具Tag获取法力消耗
	const float Cooldown = GetCooldown(Level + 1);
	const float ManaCost = GetManaCost(Level + 1);

	// 遍历伤害类型，获取每种类型的伤害值，然后拼接字符串
	FString DamageTypeString;
	for (auto& DamagePair : DamageType)
	{
		const FGameplayTag& DamageTag = DamagePair.Key;
		const FScalableFloat& DamageValue = DamagePair.Value;
		const float ScaledDamageValue = DamageValue.GetValueAtLevel(Level + 1);
		const FString TagName = FAuraGameplayTags::GetDamageCnName(DamageTag);
		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.2f</>\n"), *TagName, ScaledDamageValue);
	}

	FString Desc = FString::Printf(TEXT("<Title>火球术</>\t<Small>FIRE BOLT</>\n\n"
		"<Default>发射一道火焰，在撞击和造成伤害时爆炸 </>\n\n"
		"\t<Default>技能等级：</><Level>%d</>\n"
		"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
		"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
		"\t<Default>火球数量：</><Time>%d</>\n\n"
		"<Default>技能详细伤害描述：</>\n\n"
	), Level + 1, Cooldown, ManaCost, NumProjectiles);
	Desc += DamageTypeString;

	return Desc;
}
