// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

// 这结构体用于存储UI小部件行的数据，用来在屏幕上显示消息
USTRUCT(BlueprintType)		// 设置为蓝图类型
struct FUIWidgetRow : public FTableRowBase 	// UI小部件行，继承自FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// 设置为可编辑的任何地方，蓝图可读
	FGameplayTag MessageTag = FGameplayTag::EmptyTag;		// 消息标签

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// 设置为可编辑的任何地方，蓝图可读)
	FText MessageText = FText::GetEmpty();		// 消息文本

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// 设置为可编辑的任何地方，蓝图可读
	TSubclassOf<class UAuraUserWidget> MessageWidgetClass;		// 小部件类，这个部件我们可以在蓝图中任意定制，比如显示文本，图片等等

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// 设置为可编辑的任何地方，蓝图可读
	UTexture2D* MessageIcon = nullptr;		// 消息图标

	UPROPERTY(EditAnywhere, BlueprintReadOnly)		// 设置为可编辑的任何地方，蓝图可读
	UTexture2D* BackgroundIcon = nullptr;		// 消息背景

};

struct FOnAttributeChangeData;		// 属性改变数据

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);		// 属性改变委托，接收一个float类型的新值

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, MessageWidgetRow);		// 消息小部件行，一个参数是消息小部件行

/**
 * OverlayWidgetController, 叠加层控件控制器, 设置为BlueprintType, Blueprintable，是因为我们希望在蓝图中使用它
 */
UCLASS(BlueprintType, Blueprintable)
class ARCANE_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;		// 广播初始值
	virtual void BindCallbacksToDependencies() override;		// 绑定回调到依赖项

	/*
	 * 因为我们创建了一个动态多播委托，所以我们需要一个蓝图可分配的事件来触发它
	 * 一但我们的小部件蓝图，WBP叠加层或者任何其他东西，如果他们能够访问到这个控制器，他们就可以分配一个事件来接收这个委托
	 */

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// 设置为蓝图可分配，分类为GAS下的Attributes
	FOnAttributeChangeSignature OnHealthChanged;		// 生命值改变

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// 设置为蓝图可分配，分类为GAS下的Attributes
	FOnAttributeChangeSignature OnMaxHealthChanged;		// 最大生命值改变

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// 设置为蓝图可分配，分类为GAS下的Attributes
	FOnAttributeChangeSignature OnManaChanged;		// 法力值改变

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")		// 设置为蓝图可分配，分类为GAS下的Attributes
	FOnAttributeChangeSignature OnMaxManaChanged;		// 最大法力值改变

	UPROPERTY(BlueprintAssignable, Category="GAS|Message")		// 设置为蓝图可分配，分类为GAS下的Attributes
	FMessageWidgetRowSignature MessageWidgetRowDelegate;		// 消息小部件 行委托

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Widget Data")		// 设置为可编辑的任何地方，蓝图可读
	TObjectPtr<UDataTable> MessageWidgetDataTable;		// 消息小部件数据表

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const;		// 通过标签获取数据表行

};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag) const
{
	if (!DataTable)
	{
		return nullptr;
	}

	const FString ContextString = TEXT("");
	return DataTable->FindRow<T>(*Tag.GetTagName().ToString(), ContextString, true);
}
