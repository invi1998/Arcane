// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

/**
 * Ͷ�䣨�������ͣ�����
 */
UCLASS()
class ARCANE_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

protected:

	// ��������
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
