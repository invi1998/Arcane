// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)	// 将接口类型设置为蓝图类型
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANE_API ICombatInterface
{
	GENERATED_BODY()
	
public:
	virtual int32 GetPlayerLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	FVector GetCombatSocketLocation() const;	// 获取战斗插槽位置

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	FVector GetCombatSocketForward() const;	// 获取战斗插槽前向向量

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)		// 蓝图实现，同时蓝图可调用
	void UpdateFacingTarget(const FVector& Target);	//更新面向目标

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// 蓝图原生事件，蓝图可以重写
	UAnimMontage* GetHitReactMontage();	// 获取受击反应动画

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// 蓝图原生事件，蓝图可以重写
	UAnimMontage* GetDeathMontage();	// 获取死亡动画

	virtual void Die() = 0;	// 死亡

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	bool IsDead() const;	// 是否死亡

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	AActor* GetActor();	// 获取Actor

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	TArray<FTaggedMontage> GetAttackMontages() const;	// 获取攻击动画

	// 从TArray中随机选择一个Montage
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	FTaggedMontage GetRandomAttackMontage() const;
	
};
