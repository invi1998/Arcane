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
	FVector GetCombatSocketForward() const;	// ��ȡս�����ǰ������

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)		// ��ͼʵ�֣�ͬʱ��ͼ�ɵ���
	void UpdateFacingTarget(const FVector& Target);	//��������Ŀ��

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// ��ͼԭ���¼�����ͼ������д
	UAnimMontage* GetHitReactMontage();	// ��ȡ�ܻ���Ӧ����

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// ��ͼԭ���¼�����ͼ������д
	UAnimMontage* GetDeathMontage();	// ��ȡ��������

	virtual void Die() = 0;	// ����

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// ��ͼ�ɵ��ã���ͼԭ���¼�
	bool IsDead() const;	// �Ƿ�����

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// ��ͼ�ɵ��ã���ͼԭ���¼�
	AActor* GetActor();	// ��ȡActor

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// ��ͼ�ɵ��ã���ͼԭ���¼�
	TArray<FTaggedMontage> GetAttackMontages() const;	// ��ȡ��������

	// ��TArray�����ѡ��һ��Montage
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// ��ͼ�ɵ��ã���ͼԭ���¼�
	FTaggedMontage GetRandomAttackMontage() const;
	
};
