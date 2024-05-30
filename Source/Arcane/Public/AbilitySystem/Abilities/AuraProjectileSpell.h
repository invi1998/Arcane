// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;

/**
 * 投射（弹道类型）法术
 */
UCLASS()
class ARCANE_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

protected:

	// 激活能力
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& Tag, bool bOverridePitch = false, float PitchOverride = 0.f);		// 生成投射物

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SetProjectileClass(TSubclassOf<AAuraProjectile> NewProjectileClass);	// 设置投射物类

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;	// 投射物类

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 1;	// 投射物数量

};
