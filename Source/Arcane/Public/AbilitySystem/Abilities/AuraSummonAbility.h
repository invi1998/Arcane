// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 召唤类技能
 */
UCLASS()
class ARCANE_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Summoning")
	int32 NumMinions = 5;	// 召唤的数量

	UPROPERTY(EditAnywhere, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;	// 召唤的类

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float MinSpawnDistance = 400.f;		// 最小召唤距离

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float MaxSpawnDistance = 600.0f;	// 最大召唤距离

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float SpawnSpread = 100.0f;	// 召唤的散布(分布), 该值越大, 召唤物体的位置范围越大

	UFUNCTION(BlueprintCallable, Category = "Summoning")
	TArray<FVector> GetSpawnLocations();	// 获取召唤的位置

	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();	// 获取随机的召唤物体类
};
