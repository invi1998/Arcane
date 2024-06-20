// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

/**
 * 火爆
 */
UCLASS()
class ARCANE_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "技能|伤害")
	int32 MaxNumOfFireBalls = 13;	// 最大火球数量

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "技能|伤害")
	FScalableFloat FireBallMaxDistance;	// 火球最大飞行距离
};
