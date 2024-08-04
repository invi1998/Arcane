// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraPassiveAbility.generated.h"

/**
 * 被动技能
 */
UCLASS()
class ARCANE_API UAuraPassiveAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;	// 激活技能

	// 被动失效回调函数
	void ReceiveDeactive(const FGameplayTag& AbilityTag);

	// 被动技能数值曲线
	UPROPERTY(EditDefaultsOnly, Category="Passive")
	FScalableFloat PassiveValue;

};
