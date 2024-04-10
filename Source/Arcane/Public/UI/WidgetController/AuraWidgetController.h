// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(class APlayerController* InPlayerController, class APlayerState* InPlayerState, class UAbilitySystemComponent* InAbilitySystemComponent, class UAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController), PlayerState(InPlayerState), AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayerController> PlayerController = nullptr;		// 玩家控制器

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class APlayerState> PlayerState = nullptr;		// 玩家状态

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent = nullptr;		// 能力系统组件

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAttributeSet> AttributeSet = nullptr;		// 属性集
};

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& InParams);	// 设置控制器参数

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues(const FGameplayTag& Tag);		// 广播初始值
	virtual void BindCallbacksToDependencies();	// 绑定回调到依赖项


protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class APlayerController> PlayerController;		// 玩家控制器

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class APlayerState> PlayerState;		// 玩家状态

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;		// 能力系统组件

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class UAttributeSet> AttributeSet;		// 属性集

};
