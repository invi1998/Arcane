// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraShards.generated.h"

/**
 * 奥术碎片（Arcane Shards）技能
 */
UCLASS()
class ARCANE_API UAuraShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;

protected:
	int32 MaxNumOfShards = 11;	// 碎片数量

};
