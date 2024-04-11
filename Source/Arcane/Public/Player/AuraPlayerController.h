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
	virtual void SetupInputComponent() override;	// 该函数在控制器被创建时调用，用于设置输入组件

	void CursorTrace();	// 光标追踪，追踪鼠标指向的物体

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;	// 输入映射上下文

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;	// 移动动作

	// 有了移动动作，我们还需要一个输入处理函数
	void Move(const FInputActionValue& Value);	// 移动函数

	IEnemyInterface* LastActor;		// 上一个命中的Actor
	IEnemyInterface* ThisActor;		// 当前命中的Actor

	void AbilityInputTagPressed(FGameplayTag InputTag);	// 技能输入标签按下
	void AbilityInputTagReleased(FGameplayTag InputTag);	// 技能输入标签释放
	void AbilityInputTagHeld(FGameplayTag InputTag);	// 技能输入标签按住

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;	// 输入配置

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;	// 能力系统组件，因为在处理Input时需要用到能力系统组件，而这些Input往往会频繁触发，Cast消耗较大，所以我们在这里缓存一下

	UAuraAbilitySystemComponent* GetASC();	// 获取能力系统组件

};
