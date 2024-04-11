// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;	// �ú����ڿ�����������ʱ���ã����������������

	void CursorTrace();	// ���׷�٣�׷�����ָ�������

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;	// ����ӳ��������

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;	// �ƶ�����

	// �����ƶ����������ǻ���Ҫһ�����봦����
	void Move(const FInputActionValue& Value);	// �ƶ�����

	IEnemyInterface* LastActor;		// ��һ�����е�Actor
	IEnemyInterface* ThisActor;		// ��ǰ���е�Actor

	void AbilityInputTagPressed(FGameplayTag InputTag);	// ���������ǩ����
	void AbilityInputTagReleased(FGameplayTag InputTag);	// ���������ǩ�ͷ�
	void AbilityInputTagHeld(FGameplayTag InputTag);	// ���������ǩ��ס

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;	// ��������

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;	// ����ϵͳ�������Ϊ�ڴ���Inputʱ��Ҫ�õ�����ϵͳ���������ЩInput������Ƶ��������Cast���Ľϴ��������������ﻺ��һ��

	UAuraAbilitySystemComponent* GetASC();	// ��ȡ����ϵͳ���

};
