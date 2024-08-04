// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellButtonReassignDelegate, const FGameplayTag&, AbilityTag);	// 重新分配技能按钮委托

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ARCANE_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;	// 广播初始值

	virtual void BindCallbacksToDependencies() override;	// 绑定回调函数到依赖项

	UPROPERTY(BlueprintAssignable)
	FSpellButtonReassignDelegate SpellButtonReassignDelegate;	// 重新分配技能按钮委托

	UPROPERTY(BlueprintAssignable, Category = "gas|Attributes")		// 蓝图可调用的委托
	FOnPlayerStatChangeSignatureInt SkillPointChangeDelegate;	// 技能点改变委托

	UFUNCTION(BlueprintCallable, Category = "gas|Attributes")	// 蓝图可调用的函数
	void SpendSkillPoint(const FGameplayTag& AbilityTag);	// 消耗技能点

	UFUNCTION(BlueprintCallable, Category = "gas|Attributes")	// 蓝图可调用的函数
	void EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag);	// 装备能力

	void OnAbilitySlotChange(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& OldSlotTag) const;	// 能力槽改变
	
};
