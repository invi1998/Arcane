// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "Arcane/ArcaneLogChannels.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	if (TargetActor)
	{
		const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1);
		for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageType)
		{
			const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);	// Assign the damage value to the tag（注册伤害值到标签）
		}

		// Apply the damage to the target actor(对目标角色应用伤害)
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	}
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(FGameplayTag InDamageType, AActor* TargetActor) const
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

	return Params;
}
