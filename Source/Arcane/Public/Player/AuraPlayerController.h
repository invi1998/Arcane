// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;

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
	virtual void SetupInputComponent() override;

	void CursorTrace();	// ���׷�٣�׷�����ָ�������

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;	// ����ӳ��������

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;	// �ƶ�����

	// �����ƶ����������ǻ���Ҫһ�����봦����
	void Move(const FInputActionValue& Value);	// �ƶ�����

	TObjectPtr<IEnemyInterface> LastActor;		// ��һ�����е�Actor
	TObjectPtr<IEnemyInterface> ThisActor;		// ��ǰ���е�Actor
};
