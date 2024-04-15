// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

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

	virtual FVector GetCombatSocketLocation() const;	// 获取战斗插槽位置

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)		// 蓝图实现，同时蓝图可调用
	void UpdateFacingTarget(const FVector& Target);	//更新面向目标

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// 蓝图原生事件，蓝图可以重写
	UAnimMontage* GetHitReactMontage();	// 获取受击反应动画
	
};
