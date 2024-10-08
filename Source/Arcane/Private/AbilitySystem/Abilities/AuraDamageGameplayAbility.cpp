// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Arcane/ArcaneLogChannels.h"


void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor, FVector InRadialDamageOrigin,
	bool bOverrideKnockbackDirection, FVector KnockbackDirectionOverride, bool bOverrideDeathImpulse,
	FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride)
{
	if (IsValid(TargetActor))
	{
		// const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
		for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageType)
		{
			//const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			//UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);	// Assign the damage value to the tag（注册伤害值到标签）

			FDamageEffectParams Params;
			Params = MakeDamageEffectParamsFromClassDefaults(Pair.Key, TargetActor, InRadialDamageOrigin, bOverrideKnockbackDirection, KnockbackDirectionOverride, bOverrideDeathImpulse, DeathImpulseDirectionOverride, bOverridePitch, PitchOverride);
			// 应用伤害效果
			UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);
		}

		// Apply the damage to the target actor(对目标角色应用伤害)
		// GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	}
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(FGameplayTag InDamageType,
	AActor* TargetActor, FVector InRadialDamageOrigin, bool bOverrideKnockbackDirection,
	FVector KnockbackDirectionOverride, bool bOverrideDeathImpulse, FVector DeathImpulseDirectionOverride,
	bool bOverridePitch, float PitchOverride) const
{
	if (DamageType.Contains(InDamageType) == false)
	{
		UE_LOG(LogArcane, Warning, TEXT("MakeDamageEffectParamsFromClassDefaults: DamageType %s not found in DamageType map."), *InDamageType.ToString());
		return FDamageEffectParams();
	}
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageEffectClass = DamageEffectClass;
	Params.InstigatorASC = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.DamageType = InDamageType;
	Params.BaseDamage = DamageType[InDamageType].GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DebuffChance = DebuffChances.Contains(InDamageType) ? DebuffChances[InDamageType] : 0.f;
	Params.DebuffDamage = DebuffDamages.Contains(InDamageType) ? DebuffDamages[InDamageType] : 0.f;
	Params.DebuffFrequency = DebuffFrequencies.Contains(InDamageType) ? DebuffFrequencies[InDamageType] : 0.f;
	Params.DebuffDuration = DebuffDurations.Contains(InDamageType) ? DebuffDurations[InDamageType] : 0.f;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitudes.Contains(InDamageType) ? DeathImpulseMagnitudes[InDamageType] : 0.f;
	Params.KnockbackMagnitude = KnockbackMagnitudes.Contains(InDamageType) ? KnockbackMagnitudes[InDamageType] : 0.f;
	Params.KnockbackChance = KnockbackChances.Contains(InDamageType) ? KnockbackChances[InDamageType] : 0.f;

	if (IsValid(TargetActor))
	{
		// Set the death impulse, the forward vector of the projectile multiplied by the death impulse magnitude（设置死亡冲量，投射物的前向量乘以死亡冲量大小）
		FRotator KnockbackRotator = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			KnockbackRotator.Pitch = PitchOverride;
		}
		const FVector KnockbackDirection = KnockbackRotator.Vector();
		if (!bOverrideKnockbackDirection)
		{
			Params.KnockbackImpulse = KnockbackDirection * KnockbackDirectionOverride;
		}
		if (!bOverrideDeathImpulse)
		{
			Params.DeathImpulse = KnockbackDirection * DeathImpulseDirectionOverride;
		}
	}

	if (bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		Params.KnockbackImpulse = KnockbackDirectionOverride * Params.KnockbackMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			Params.KnockbackImpulse = KnockbackRotation.Vector() * Params.KnockbackMagnitude;
		}
	}

	if (bRadialDamage.Contains(InDamageType) && bRadialDamage[InDamageType])
	{
		// 如果设置了径向伤害，那么我们需要设置径向伤害的参数
		Params.bIsRadialDamage = true;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius.Contains(InDamageType) ? RadialDamageInnerRadius[InDamageType] : 0.f;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius.Contains(InDamageType) ? RadialDamageOuterRadius[InDamageType] : 0.f;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
	}

	return Params;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel(FGameplayTag InDamageType) const
{
	const int32 Level = GetAbilityLevel();

	return DamageType.Contains(InDamageType) ? DamageType[InDamageType].GetValueAtLevel(Level) : 0.f;
}
