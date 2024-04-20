// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANE_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HighlightActor() = 0;		// 高亮显示敌人
	virtual void UnHighlightActor() = 0;	// 取消高亮显示敌人

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy Interface")
	void SetCombatTarget(AActor* InCombatTarget);	// 设置攻击目标

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy Interface")
	AActor* GetCombatTarget() const;	// 获取攻击目标
};
