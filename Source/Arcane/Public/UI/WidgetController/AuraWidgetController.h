// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAbilityInfo;

class APlayerController;
class APlayerState;
class UAbilitySystemComponent;
class UAttributeSet;

class AAuraPlayerController;
class AAuraPlayerState;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);		// 属性改变委托，接收一个float类型的新值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangeSignatureInt, int32, NewValue);		// 玩家状态改变委托，接收一个int32类型的新值
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, AbilityInfo);		// 能力信息委托，一个参数是能力信息)

// 技能释放状态
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityCastStateChangeSignature, const FGameplayTag&, AbilityTag, bool, bIsCasting);		// 技能释放状态改变委托，两个参数是能力标签和是否正在释放

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* InPlayerController, APlayerState* InPlayerState, UAbilitySystemComponent* InAbilitySystemComponent, UAttributeSet* InAttributeSet)
		: PlayerController(InPlayerController), PlayerState(InPlayerState), AbilitySystemComponent(InAbilitySystemComponent), AttributeSet(InAttributeSet) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;		// 玩家控制器

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;		// 玩家状态

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;		// 能力系统组件

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;		// 属性集
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
	virtual void BroadcastInitialValues();		// 广播初始值

	virtual void BindCallbacksToDependencies();	// 绑定回调到依赖项

	UPROPERTY(BlueprintAssignable, Category = "GAS|Abilities")		// 设置为蓝图可分配，分类为GAS下的Abilities
	FAbilityInfoSignature AbilityInfoDelegate;		// 能力信息委托

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")		// 设置为蓝图可分配，分类为GAS下的Attributes
	FOnAbilityCastStateChangeSignature AbilityCastStateChangeDelegate;		// 技能释放状态改变委托

	void BroadcastAbilityInfo();		// 广播能力信息

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widget Data")		// 设置为可编辑的任何地方，蓝图可读
	TObjectPtr<UAbilityInfo> AbilityInformation;		// 能力信息

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;		// 玩家控制器

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;		// 玩家状态

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;		// 能力系统组件

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;		// 属性集

	// 项目类型的玩家控制器，玩家状态，能力系统组件，属性集
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;		// 玩家状态

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;		// 属性集

	AAuraPlayerController* GetAuraPC();		// 获取Aura玩家控制器
	AAuraPlayerState* GetAuraPS();		// 获取Aura玩家状态
	UAuraAbilitySystemComponent* GetAuraASC();		// 获取Aura能力系统组件
	UAuraAttributeSet* GetAuraAS();		// 获取Aura属性集


};
