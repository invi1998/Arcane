// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)	// ���ӿ���������Ϊ��ͼ����
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

	virtual FVector GetCombatSocketLocation() const;	// ��ȡս�����λ��

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)		// ��ͼʵ�֣�ͬʱ��ͼ�ɵ���
	void UpdateFacingTarget(const FVector& Target);	//��������Ŀ��
};
