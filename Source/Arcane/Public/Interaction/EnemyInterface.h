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
	virtual void HighlightActor() = 0;		// ������ʾ����
	virtual void UnHighlightActor() = 0;	// ȡ��������ʾ����

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy Interface")
	void SetCombatTarget(AActor* InCombatTarget);	// ���ù���Ŀ��

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy Interface")
	AActor* GetCombatTarget() const;	// ��ȡ����Ŀ��
};
