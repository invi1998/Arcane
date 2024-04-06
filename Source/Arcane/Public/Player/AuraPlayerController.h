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

	void CursorTrace();	// 光标追踪，追踪鼠标指向的物体

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;	// 输入映射上下文

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;	// 移动动作

	// 有了移动动作，我们还需要一个输入处理函数
	void Move(const FInputActionValue& Value);	// 移动函数

	TObjectPtr<IEnemyInterface> LastActor;		// 上一个命中的Actor
	TObjectPtr<IEnemyInterface> ThisActor;		// 当前命中的Actor
};
