// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);		// 设置控件控制器

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;		// 控件控制器

protected:
	UFUNCTION(BlueprintImplementableEvent)		// 蓝图实现事件，用于设置控件控制器
	void WidgetControllerSet();		// 设置控件控制器
	
};
