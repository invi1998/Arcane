// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IHilightInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;		// 样条曲线组件
class UDamageTextComponent;
class UNiagaraSystem;
class AMagicCircle;

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

	FHitResult GetCursorHitResult() const;	// 获取光标碰撞结果

	UFUNCTION(Client, Reliable)
	void ShowDamageText(float Damage, ACharacter* Target, bool bBlockedHit, bool bCriticalHit);	// 显示伤害文本

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle();	// 显示法环

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();	// 隐藏法环

	void SetDecalMaterial(UMaterialInterface* DecalMaterial) const;	// 设置贴花材质

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;	// 该函数在控制器被创建时调用，用于设置输入组件

	void CursorTrace();	// 光标追踪，追踪鼠标指向的物体

	FHitResult CursorHitResult;	// 创建一个碰撞结果

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;	// 输入映射上下文

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;	// Shit动作

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;	// 移动动作

	// 有了移动动作，我们还需要一个输入处理函数
	void Move(const FInputActionValue& Value);	// 移动函数

	void ShiftPressed() { bShiftKeyDown = true; }	// Shift按下
	void ShiftReleased() { bShiftKeyDown = false; }	// Shift释放
	bool bShiftKeyDown = false;

	IHilightInterface* LastActor;		// 上一个命中的Actor
	IHilightInterface* ThisActor;		// 当前命中的Actor
	
	void AbilityInputTagPressed(FGameplayTag InputTag);	// 技能输入标签按下

	void AbilityInputTagReleased(FGameplayTag InputTag);	// 技能输入标签释放

	void AbilityInputTagHeld(FGameplayTag InputTag);	// 技能输入标签按住

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;	// 输入配置

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;	// 能力系统组件，因为在处理Input时需要用到能力系统组件，而这些Input往往会频繁触发，Cast消耗较大，所以我们在这里缓存一下

	UAuraAbilitySystemComponent* GetASC();	// 获取能力系统组件

	/*
	 * 寻路避障
	 */
	FVector CashedDestination = FVector::ZeroVector;	// 缓存的目标位置

	float FollowTime = 0.1f;	// 跟随时间

	float ShortPressThreshold = 0.5f;	// 短按阈值

	bool bAutoRunning = false;	// 是否自动寻路

	bool bTargeting = false;	// 是否在瞄准

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.0f;	// 自动寻路接受半径

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;	// 样条曲线组件

	void AutoRun();	// 自动寻路

	/*
	 * Damage Text
	 */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;	// 点击特效

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;	// 法环类

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;	// 法环

	void UpdateMagicCircleLocation() const;	// 更新法环位置

};
