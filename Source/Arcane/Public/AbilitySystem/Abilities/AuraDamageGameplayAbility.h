// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);	// 造成伤害

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(FGameplayTag InDamageType, AActor* TargetActor = nullptr) const;	// 从类默认值创建伤害效果参数

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;		// 伤害效果

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageType;	// 伤害类型

	// Debuff效果的各项参数
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, float> DebuffChances;		// Debuff几率

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, float> DebuffDamages;		// Debuff伤害

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, float> DebuffFrequencies;	// Debuff频率

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<FGameplayTag, float> DebuffDurations;		// Debuff持续时间

};
