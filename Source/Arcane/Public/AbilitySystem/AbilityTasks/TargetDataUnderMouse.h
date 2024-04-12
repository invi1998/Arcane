// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

// 广播委托，广播内容为鼠标位置，参数为FVector类型
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FVector&, Data);

/**
 * 
 */
UCLASS()
class ARCANE_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public: 
	/**
	 * @brief 创建此任务实例并返回的工厂函数
	 * @param OwningAbility	拥有此任务的能力
	 * @return 
	 */
	// UFUNCTION，蓝图调用的函数，返回一个UTargetDataUnderMouse*类型的指针，分类为"Ability|Tasks"，
	// DisplayName为"TargetDataNuderMouse"，表示在蓝图中显示的名称为"TargetDataNuderMouse"
	// 隐藏Pin为OwningAbility，表示不在蓝图中显示OwningAbility这个Pin（引脚）
	// DefaultToSelf为OwningAbility，表示默认值为OwningAbility
	// BlueprintInternalUseOnly为true，表示只能在蓝图中使用
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName="TargetDataNuderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;	// 声明一个委托，委托内容为FVector类型，这将成为节点上的输出执行引脚

private:
	virtual void Activate() override;	// 重写Activate函数，激活任务
	
};
