// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LootTiers|Spawning")
	TSubclassOf<AActor> LootClass;				// 掉落物品类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LootTiers|Spawning")
	float ChanceToDrop = 0.f;					// 掉落几率

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LootTiers|Spawning")
	int32 MaxNumberToSpawn = 5;					// 最大掉落数量

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LootTiers|Spawning")
	bool bLootLevelOverride = true;				// 是否覆盖掉落等级（如果为true，则将用角色的等级覆盖掉落物品的等级）
};

/**
 * 战利品掉落
 */
UCLASS()
class ARCANE_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:
	/** 获取掉落物品 */
	UFUNCTION(BlueprintCallable, Category="LootTiers")
	TArray<FLootItem> GetLootItems() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="LootTiers")
	TArray<FLootItem> LootItems;	// 掉落物品

	
};
