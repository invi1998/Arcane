// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:


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
