// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

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

	UPROPERTY(BlueprintAssignable, Category = "gas|Attributes")		// 蓝图可调用的委托
	FOnPlayerStatChangeSignatureInt SkillPointChangeDelegate;	// 技能点改变委托
	
};
