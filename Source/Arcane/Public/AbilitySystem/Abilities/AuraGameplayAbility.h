// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;	// 启动输入标签

	virtual FString GetDescription(int32 Level);

	virtual FString GetNextLevelDescription(int32 Level);

	static FString GetLockedDescription(int32 Level);

	float GetManaCost(float Level = 1.f);

	float GetCooldown(float Level = 1.f);

};
