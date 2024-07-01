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
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;	// 启动输入标签

	UPROPERTY(BlueprintReadWrite, Category="State")
	FGameplayTag AbilityStateTag;	// 技能状态标签

	virtual FString GetDescription(int32 Level);

	virtual FString GetNextLevelDescription(int32 Level);

	static FString GetLockedDescription(int32 Level);

	float GetManaCost(float Level = 1.f);

	float GetCooldown(float Level = 1.f);

};
