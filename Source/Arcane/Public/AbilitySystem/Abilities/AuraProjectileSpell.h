// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;

/**
 * 投射（弹道类型）法术
 */
UCLASS()
class ARCANE_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	// 激活能力
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& Tag, bool bOverridePitch = false, float PitchOverride = 0.f);		// 生成投射物

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAuraProjectile> ProjectileClass;	// 投射物类

};
