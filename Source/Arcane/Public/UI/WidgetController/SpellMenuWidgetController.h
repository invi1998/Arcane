// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues(const FGameplayTag& Tag) override;	// 广播初始值

	virtual void BindCallbacksToDependencies() override;	// 绑定回调函数到依赖项
	
};
