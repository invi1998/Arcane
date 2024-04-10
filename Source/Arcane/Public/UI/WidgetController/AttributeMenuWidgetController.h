// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FAuraAttributeInfo;
class UAttributeInfo;

// 创建一个能广播AuraAttributeSet属性值的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ARCANE_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues(const FGameplayTag& Tag) override;
	virtual void BindCallbacksToDependencies() override;	// 绑定回调函数到依赖项

	void BroadcastAttributeInfo(const FGameplayTag& Tag);	// 广播属性信息

	UPROPERTY(BlueprintAssignable, Category = "gas|Attributes")		// 蓝图可调用的委托
	FAttributeInfoSignature AttributeInfoDelegate;	// 属性信息委托

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AuraAttributeInfo;	// 属性信息

};
