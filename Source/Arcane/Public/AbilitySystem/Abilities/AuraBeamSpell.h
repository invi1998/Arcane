// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerPlayer();

	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTarget(TArray<AActor*>& OutAdditionalTargets);		// 存储额外的目标

	UFUNCTION(BlueprintImplementableEvent)	// 蓝图实现事件
	void PrimaryBeamTargetDied(AActor* DeadActor);	// 主要目标死亡

	UFUNCTION(BlueprintImplementableEvent)	// 蓝图实现事件
	void AdditionalBeamTargetDied(AActor* DeadActor);	// 额外目标死亡

	UFUNCTION(BlueprintCallable)
	float GetMaxSpellContinuationTime() const;	// 获取技能持续时间

	virtual FString GetDescription(int32 Level) override;

	virtual FString GetNextLevelDescription(int32 Level) override;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;		// 鼠标点击位置

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;	// 鼠标点击的Actor

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;	// 玩家控制器

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;	// 执行技能的角色

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	int32 MaxNumOfTargets = 5;	// 最大目标数量

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	float BeamRange = 500.f;	// 射线范围

	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	float MinSpellCooldownTime = 0.5f;	// 技能进入冷却的最小时间

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	FScalableFloat MaxSpellContinuationTime;	// 技能最大持续时间

	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	bool IsWeaponCast = false;	// 是否是武器释放法术

};
