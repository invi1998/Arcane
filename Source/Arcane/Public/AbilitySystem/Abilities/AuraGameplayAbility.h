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
	FGameplayTag StartupInputTag;	// ∆Ù∂Ø ‰»Î±Í«©

	virtual FString GetDescription(int32 Level);

	virtual FString GetNextLevelDescription(int32 Level);

	static FString GetLockedDescription(int32 Level);
};
