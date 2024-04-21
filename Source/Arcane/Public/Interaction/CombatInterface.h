// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

class UAnimMontage;

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// ��ͼ�ɵ��ã���ͼԭ���¼�
	FVector GetCombatSocketLocation() const;	// ��ȡս�����λ��

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// ��ͼ�ɵ��ã���ͼԭ���¼�
	FRotator GetCombatSocketRotation() const;	// ��ȡս�������ת

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)		// ��ͼʵ�֣�ͬʱ��ͼ�ɵ���
	void UpdateFacingTarget(const FVector& Target);	//��������Ŀ��

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// ��ͼԭ���¼�����ͼ������д
	UAnimMontage* GetHitReactMontage();	// ��ȡ�ܻ���Ӧ����

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// ��ͼԭ���¼�����ͼ������д
	UAnimMontage* GetDeathMontage();	// ��ȡ��������

	virtual void Die() = 0;	// ����
	
};
