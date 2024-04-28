// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayEffectSpec;
class UAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownChangeSignature, float, CooldownTimeRemaining);

/**
 * 技能冷却时间异步任务
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))	// 蓝图类型，暴露异步代理
class ARCANE_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)	// 蓝图绑定事件
	FOnCooldownChangeSignature CooldownStart;	// 冷却开始

	UPROPERTY(BlueprintAssignable)	// 蓝图绑定事件
	FOnCooldownChangeSignature CooldownEnd;	// 冷却结束

	// 该函数必须是蓝图调用的，而且因为这是一个异步任务，所以这个函数是一个内部函数，这与AbilityTask一样，会在蓝图中创建一个异步任务节点
	// 该函数执行该节点的时候将会创建一个实例，所以，为了说明这个函数是一个内部函数，我们需要使用meta标签
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))	// 蓝图调用
	static UWaitCooldownChange* WaitCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);	// 创建任务

	UFUNCTION(BlueprintCallable)
	void EndTask();	// 结束任务时调用，清理事件绑定


protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY()
	FGameplayTag CooldownTag;

	void CooldownChanged(const FGameplayTag InCooldownTag, int32 NewCount);	// 冷却时间改变时调用

	void OnActiveEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveHandle);	// 添加冷却效果时调用

};

