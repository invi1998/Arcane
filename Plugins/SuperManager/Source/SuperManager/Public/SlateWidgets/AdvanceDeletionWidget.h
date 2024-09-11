// Copyright INVI_1998

#pragma once

#include "Widgets/SCompoundWidget.h"


class SAdvanceDeletionTab : public SCompoundWidget
{


	// SLATE_BEGIN_ARGS 是一个宏，用于定义构造函数的参数列表
	SLATE_BEGIN_ARGS(SAdvanceDeletionTab) {}

	// SLATE_ARGUMENT 宏用于定义构造函数的参数
	SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, AssetDataList)
	SLATE_ARGUMENT(FString, CurrentSelectedFolderPath)

	SLATE_END_ARGS()

public:
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	void FixUpRedirectors();	// 修复重定向器

private:
	TArray<TSharedPtr<FAssetData>> AssetDataUnderSelectedFolder;	// 选中文件夹下的资产数据
	TArray<TSharedPtr<FAssetData>> DisplayedAssetDataList;	// 显示的资产数据列表

	void OnDoubleClickAssetItem(TSharedPtr<FAssetData> AssetData);	// 双击资产项事件
	void OnClickAssetItem(TSharedPtr<FAssetData, ESPMode::ThreadSafe> AssetData);	// 点击资产项事件（单击表行选中或者取消选中）
	TSharedRef<SListView<TSharedPtr<FAssetData>>> OnConstructAssetListView();	// 构建资产列表视图
	TSharedPtr<SListView<TSharedPtr<FAssetData>>> ConstructedAssetListView;	// 资产列表视图
	void RefreshAssetListView();	// 刷新资产列表视图

#pragma region ComboBoxForListingConditions

	TSharedRef<SComboBox<TSharedPtr<FString>>> OnConstructComboBox();	// 构建下拉框

	TSharedRef<SWidget> OnConstructComboBoxWidget(TSharedPtr<FString> Item);	// 构建下拉框部件

	void OnComboBoxSelectionChanged(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo);	// 下拉框选项改变事件

	TSharedRef<SButton> GenerateComboBoxHelper();	// 生成下拉框帮助

	TSharedPtr<STextBlock> ComboDisplayTextBlock;	// 下拉框文本部件

	TArray<TSharedPtr<FString>> ComboBoxSourceItems;	// 下拉框选项列表

#pragma endregion

#pragma region RowWidgetsForAssetListViews

	// 生成资产列表的行
	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAssetData> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedRef<SCheckBox> OnGenerateCheckBox(const TSharedPtr<FAssetData>& Item);

	void OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> Item);

	TSharedRef<STextBlock> OnGenerateTextBlockForRowWidget(const FString& TextContent, const FSlateFontInfo& FontInfo);

	FReply OnRowDeleteButtonClicked(TSharedPtr<FAssetData> AssetData);

	TSharedRef<SButton> OnGenerateButtonForRowWidget(const TSharedPtr<FAssetData>& Item);

#pragma endregion


#pragma region TabButtons
	// 全选按钮点击事件, 同时还需要传入按钮本身的引用
	FReply OnSelectAllButtonClicked();		// 全选按钮点击事件
	FReply OnDeselectAllButtonClicked();	// 取消全选按钮点击事件
	FReply OnDeleteSelectedButtonClicked();	// 删除选中按钮点击事件

	TSharedRef<SButton> OnGenerateSelectAllButton();	// 生成全选按钮
	TSharedRef<SButton> OnGenerateDeselectAllButton();		// 生成取消全选按钮
	TSharedRef<SButton> OnGenerateDeleteSelectedButton();	// 生成删除按钮

	TSharedRef<STextBlock> ConstructTextForTabButtons(const FText& TextContent);	// 构建选项卡按钮文本

	TArray<TSharedPtr<FAssetData>> SelectedAssetDataList;	// 选中的资产数据列表
	TArray<TSharedRef<SCheckBox>> CheckBoxList;	// 复选框列表

	TMap<TSharedPtr<FAssetData>, TSharedRef<SCheckBox>> AssetDataToCheckBoxMap;	// 资产数据到复选框的映射

#pragma endregion


};

