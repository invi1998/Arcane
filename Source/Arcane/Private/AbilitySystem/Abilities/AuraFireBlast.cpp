// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{

	// 根据Tag获取法力消耗
	const float Cooldown = GetCooldown(Level);
	const float ManaCost = GetManaCost(Level);
	const float MaxFlyDist = FireBallMaxDistance.GetValueAtLevel(Level);

	// 遍历伤害类型，获取每种类型的伤害值，然后拼接字符串
	FString DamageTypeString;

	for (auto& DamagePair : DamageType)
	{
		const FGameplayTag& DamageTag = DamagePair.Key;
		const FScalableFloat& DamageValue = DamagePair.Value;
		const float ScaledDamageValue = DamageValue.GetValueAtLevel(Level);
		const FString TagName = FAuraGameplayTags::GetDamageCnName(DamageTag);
		DamageTypeString += FString::Printf(TEXT("\t<Default>%s：</><Damage>%.0f</>\n"), *TagName, ScaledDamageValue);

		// 如果该技能有Debuff效果，那么我们需要在描述中添加Debuff效果
		if (DebuffChances.Contains(DamageTag))
		{
			const float DebuffChance = DebuffChances[DamageTag];
			const float DebuffDamage = DebuffDamages[DamageTag];
			const float DebuffFrequency = DebuffFrequencies[DamageTag];
			const float DebuffDuration = DebuffDurations[DamageTag];
			DamageTypeString += FString::Printf(TEXT("\t<Default>该技能命中敌人时会照成灼烧效果，灼烧伤害为</><Damage>%.0f</><Default>，灼烧频率为</><Time>%.1f</><Default>，灼烧持续时间为</><Time>%.0f</>\n"), DebuffDamage, DebuffFrequency, DebuffDuration);
		}

		DamageTypeString += TEXT("\n");

	}

	const int32 RealMaxNumOfTargets = FMath::Min(MaxNumOfFireBalls, Level);

	FString Desc = FString::Printf(TEXT("<Title>火爆</>\t<Small>Fire Blast</>\n\n"
		"<Default>在角色周围生成数个火球，火球会像四周散开到达最大距离后会折返回来。散开和折返均会造成伤害，同时火球数量和折返距离会随等级变化</>\n\n"
		"\t<Default>技能等级：</><Level>%d</>\n"
		"\t<Default>冷却时间：</><Cooldown>%.1f s</>\n"
		"\t<Default>法力消耗：</><ManaCast>%.1f</>\n"
		"\t<Default>火球数量：</><Time>%d</>\n\n"
		"\t<Default>火球飞行最大距离：</><Time>%.0f</>\n\n"
		"<Default>技能详细伤害描述：</>\n"
	), Level, Cooldown, ManaCost, RealMaxNumOfTargets, MaxFlyDist);
	Desc += DamageTypeString;

	return Desc;

}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	return GetDescription(Level + 1);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	const FVector TemForward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	TArray<FRotator> FireBallRotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(TemForward, FVector::UpVector, 360.f, 4);

	TArray<AAuraFireBall*> FireBalls;

	for (const FRotator& Rot : FireBallRotators)
	{
		const FVector SpawnLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + TemForward * 100.f;
		const FTransform SpawnTransform = FTransform(Rot, SpawnLocation);
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		// 生成火球
		
		for (auto& DamagePair : DamageType)
		{
			FGameplayTag DamageTag = DamagePair.Key;
			FScalableFloat DamageValue = DamagePair.Value;
			FDamageEffectParams DamageParams = MakeDamageEffectParamsFromClassDefaults(DamageTag);
			FireBall->DamageEffectParams.Add(DamageTag, DamageParams);
		}

		FireBall->ReturnTarget = GetAvatarActorFromActorInfo();
		
		FireBalls.Add(FireBall);

		FireBall->MaxFlyDistance = FireBallMaxDistance.GetValueAtLevel(GetAbilityLevel());

		FireBall->FinishSpawning(SpawnTransform);
	}

	return FireBalls;
}
