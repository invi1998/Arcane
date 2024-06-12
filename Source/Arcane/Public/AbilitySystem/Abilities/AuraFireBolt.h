// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable, Category = "Aura Ability")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, AActor* HomingTarget, bool bOverridePitch = false, float Pitch = 0.0f);


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Aura Ability")
	float ProjectileSpread = 90.0f;		// 投射物散射角度

	UPROPERTY(EditDefaultsOnly, Category = "Aura Ability")
	int32 MaxProjectileCount = 10;		// 最大投射物数量

	UPROPERTY(EditDefaultsOnly, Category = "Aura Ability")
	float HomingAccelerationMin = 1600.0f;	// 导弹最小加速度

	UPROPERTY(EditDefaultsOnly, Category = "Aura Ability")
	float HomingAccelerationMax = 3200.f;	// 导弹最大加速度

	UPROPERTY(EditDefaultsOnly, Category = "Aura Ability")
	bool bIsHomingProjectile = true;
};
