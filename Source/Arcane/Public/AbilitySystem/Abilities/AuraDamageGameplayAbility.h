// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, 
		FVector InRadialDamageOrigin = FVector::ZeroVector, 
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f);	// 造成伤害

	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(FGameplayTag InDamageType,
		AActor* TargetActor = nullptr, 
		FVector InRadialDamageOrigin = FVector::ZeroVector, 
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;	// 从类默认值创建伤害效果参数

	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel(FGameplayTag InDamageType) const;	// 获取指定等级的伤害

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;		// 伤害效果

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageType;	// 伤害类型

	// Debuff效果的各项参数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> DebuffChances;		// Debuff几率

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> DebuffDamages;		// Debuff伤害

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> DebuffFrequencies;	// Debuff频率

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> DebuffDurations;		// Debuff持续时间

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> DeathImpulseMagnitudes;	// 死亡冲量大小（因为我们希望击杀敌人时有一个冲量效果）这里我们只设置了冲凉量大小，没有指定其方向，因为方向是在具体受击时决定的

	// 击退效果的各项参数
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> KnockbackMagnitudes;	// 击退大小

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> KnockbackChances;		// 击退几率

	/*
	 * 径向伤害效果的各项参数
	 */

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, bool> bRadialDamage;	// 是否是径向伤害

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> RadialDamageInnerRadius;	// 径向伤害内半径

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, float> RadialDamageOuterRadius;	// 径向伤害外半径


};
