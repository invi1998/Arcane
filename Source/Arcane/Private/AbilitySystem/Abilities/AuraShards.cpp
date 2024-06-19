// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraShards.h"

#include "AuraGameplayTags.h"


FString UAuraShards::GetDescription(int32 Level)
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
		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.0f</>\n"), *TagName, ScaledDamageValue);

		// 如果该技能有击退效果，那么我们需要在描述中添加击退效果
		if (KnockbackChances.Contains(DamageTag))
		{
			const float KnockbackChance = KnockbackChances[DamageTag];
			const float KnockbackDistance = KnockbackMagnitudes[DamageTag];
			DamageTypeString += FString::Printf(TEXT("\t<Default>该技能生成时会在中心照成</><Chance>%.0f</><Default>码范围击退</>\n"), KnockbackDistance);
		}

		// 如果该技能有Debuff效果，那么我们需要在描述中添加Debuff效果
		if (DebuffChances.Contains(DamageTag))
		{
			const float DebuffChance = DebuffChances[DamageTag];
			const float DebuffDamage = DebuffDamages[DamageTag];
			const float DebuffFrequency = DebuffFrequencies[DamageTag];
			const float DebuffDuration = DebuffDurations[DamageTag];
			DamageTypeString += FString::Printf(TEXT("\t<Default>该技能有</><Chance>%.0f</><Default>概率触发灼烧效果，灼烧伤害为</><Damage>%.0f</><Default>，灼烧频率为</><Time>%.1f</><Default>，灼烧持续时间为</><Time>%.0f</>\n"), DebuffChance, DebuffDamage, DebuffFrequency, DebuffDuration);
		}

		DamageTypeString += TEXT("\n");

	}

	const int32 RealMaxNumOfTargets = FMath::Min(MaxNumOfShards, Level);

	FString Desc = FString::Printf(TEXT("<Title>奥术碎片</>\t<Small>Arcane Shards</>\n\n"
		"<Default>在法环位置生成一道升起的熔岩，照成伤害和击退。伤害和熔岩数量随等级提升变化</>\n\n"
		"\t<Default>技能等级：</><Level>%d</>\n"
		"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
		"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
		"\t<Default>熔岩数量：</><Time>%d</>\n\n"
		"<Default>技能详细伤害描述：</>\n"
	), Level, Cooldown, ManaCost, RealMaxNumOfTargets);
	Desc += DamageTypeString;

	return Desc;
}

FString UAuraShards::GetNextLevelDescription(int32 Level)
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

		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.0f</>\n"), *TagName, ScaledDamageValue);

		// 如果该技能有击退效果，那么我们需要在描述中添加击退效果
		if (KnockbackChances.Contains(DamageTag))
		{
			const float KnockbackDistance = KnockbackMagnitudes[DamageTag];
			DamageTypeString += FString::Printf(TEXT("\t<Default>该技能生成时会在中心照成</><Chance>%.0f</><Default>码范围击退</>\n"), KnockbackDistance);
		}
	}

	const int32 RealMaxNumOfTargets = FMath::Min(MaxNumOfShards, Level + 1);

	FString Desc = FString::Printf(TEXT("<Title>奥术碎片</>\t<Small>Arcane Shards</>\n\n"
			"<Default>在法环位置生成一道升起的熔岩，照成伤害和击退。伤害和熔岩数量随等级提升变化</>\n\n"
			"\t<Default>技能等级：</><Level>%d</>\n"
			"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
			"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
			"\t<Default>熔岩数量：</><Time>%d</>\n\n"
			"<Default>技能详细伤害描述：</>\n"
		), Level + 1, Cooldown, ManaCost, RealMaxNumOfTargets);

	Desc += DamageTypeString;

	return Desc;
}
