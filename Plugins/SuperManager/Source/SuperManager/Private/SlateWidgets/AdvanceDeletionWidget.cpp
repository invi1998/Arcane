// Copyright INVI_1998

#include "SlateWidgets/AdvanceDeletionWidget.h"

#include "SlateBasics.h"
#include "DebugHeader.h"
#include "SuperManager.h"


#define LIST_ALL TEXT("List All Assets")
#define LIST_UNUSED TEXT("List Unused Assets")
#define LIST_SAME_NAME TEXT("List Same Name Assets")


void SAdvanceDeletionTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;	// 设置焦点

	AssetDataUnderSelectedFolder.Empty();
	DisplayedAssetDataList.Empty();

	AssetDataUnderSelectedFolder = InArgs._AssetDataList;	// 获取选中文件夹下的资产数据
	DisplayedAssetDataList = AssetDataUnderSelectedFolder;	// 显示的资产数据列表

	CheckBoxList.Empty();
	SelectedAssetDataList.Empty();

	AssetDataToCheckBoxMap.Empty();

	FSlateFontInfo TitleFontInfo = FCoreStyle::Get().GetFontStyle("EmbossedText");
	TitleFontInfo.Size = 24;

	FSlateFontInfo NormalFontInfo = FCoreStyle::Get().GetFontStyle("EmbossedText");
	NormalFontInfo.Size = 12;

	FSlateFontInfo SmallFontInfo = FCoreStyle::Get().GetFontStyle("EmbossedText");
	SmallFontInfo.Size = 8;

	ComboBoxSourceItems.Empty();

	ComboBoxSourceItems.Add(MakeShared<FString>(LIST_ALL));
	ComboBoxSourceItems.Add(MakeShared<FString>(LIST_UNUSED));
	ComboBoxSourceItems.Add(MakeShared<FString>(LIST_SAME_NAME));

	ChildSlot
		[
			// 垂直布局
			SNew(SVerticalBox)

				// 第一行 （标题）
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
						.Text(FText::FromString("Advance Deletion"))
						.Font(TitleFontInfo)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FLinearColor::White)
				]

				// 展示当前选中的文件夹路径
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5.f)
				[
					SNew(STextBlock)
						.Text(FText::FromString("Current Selected Folder Path: " + InArgs._CurrentSelectedFolderPath))
						.Font(SmallFontInfo)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FLinearColor::Gray)
				]

				// 分割线
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5.f)
				[
					SNew(SSeparator)
						.Orientation(Orient_Horizontal)
						.Thickness(2.f)
						.ColorAndOpacity(FLinearColor::White)
				]

				// 第二行 （下拉菜单，用于筛选资产）
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

						+ SHorizontalBox::Slot()
						.AutoWidth()
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						.FillWidth(0.3)
						[
							SNew(SHorizontalBox)

								// ComboBox
								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(5.f)
								[
									OnConstructComboBox()
								]

								// 帮助文本
								+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(5.f)
								[
									GenerateComboBoxHelper()
								]
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						.HAlign(HAlign_Right)
						.VAlign(VAlign_Center)
						.FillWidth(0.7)
						[
							SNew(SHorizontalBox)

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.HAlign(HAlign_Right)
								.VAlign(VAlign_Center)
								.Padding(5.f)
								[
									OnGenerateSelectAllButton()
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.HAlign(HAlign_Right)
								.VAlign(VAlign_Center)
								.Padding(5.f)
								[
									OnGenerateDeselectAllButton()
								]

								+ SHorizontalBox::Slot()
								.AutoWidth()
								.HAlign(HAlign_Right)
								.VAlign(VAlign_Center)
								.Padding(5.f)
								[
									OnGenerateDeleteSelectedButton()
								]
						]


				]

				// 分割线
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5.f)
				[
					SNew(SSeparator)
						.Orientation(Orient_Horizontal)
						.Thickness(2.f)
						.ColorAndOpacity(FLinearColor::White)
				]

				// 第三行 （表头）
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SHorizontalBox)

						// 第一列：复选框
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						.FillWidth(0.05)
						.Padding(5.f)
						[
							SNew(STextBlock)
								.Text(FText::FromString(""))
								.Font(NormalFontInfo)
								.Justification(ETextJustify::Center)
								.ColorAndOpacity(FLinearColor::White)
						]

						// 分割线
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SSeparator)
								.Orientation(Orient_Vertical)
								.Thickness(2.f)
								.ColorAndOpacity(FLinearColor::White)
						]

						// 第二列：资产类型
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						.FillWidth(0.25)
						.Padding(5.f)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Asset Type"))
								.Font(NormalFontInfo)
								.Justification(ETextJustify::Center)
								.ColorAndOpacity(FLinearColor::White)
						]

						// 分割线
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SSeparator)
								.Orientation(Orient_Vertical)
								.Thickness(2.f)
								.ColorAndOpacity(FLinearColor::White)
						]

						// 第三列：资产名称
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						.FillWidth(0.2)
						.Padding(5.f)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Asset Name"))
								.Font(NormalFontInfo)
								.Justification(ETextJustify::Center)
								.ColorAndOpacity(FLinearColor::White)
						]

						// 分割线
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SSeparator)
								.Orientation(Orient_Vertical)
								.Thickness(2.f)
								.ColorAndOpacity(FLinearColor::White)
						]

						// 第四列：资产路径
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Left)
						.VAlign(VAlign_Center)
						.FillWidth(0.4)
						.Padding(5.f)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Asset Path"))
								.Font(NormalFontInfo)
								.Justification(ETextJustify::Center)
								.ColorAndOpacity(FLinearColor::White)
						]

						// 分割线
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SNew(SSeparator)
								.Orientation(Orient_Vertical)
								.Thickness(2.f)
								.ColorAndOpacity(FLinearColor::White)
						]

						// 第五列：删除按钮
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.FillWidth(0.1)
						.Padding(5.f)
						[
							SNew(STextBlock)
								.Text(FText::FromString("Action"))
								.Font(NormalFontInfo)
								.Justification(ETextJustify::Center)
								.ColorAndOpacity(FLinearColor::White)
						]
				]

				// 分割线
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(2.f)
				[
					SNew(SSeparator)
						.Orientation(Orient_Horizontal)
						.Thickness(2.f)
						.ColorAndOpacity(FLinearColor::White)
				]


				// 第四行 （实际的资产列表，用于显示资产，滚动条）
				+ SVerticalBox::Slot()
				.VAlign(VAlign_Fill)		// 垂直填充
				[
					SNew(SScrollBox)
						.Orientation(Orient_Vertical)
						.ScrollBarAlwaysVisible(false)
						+ SScrollBox::Slot()
						[
							OnConstructAssetListView()
						]
				]

		];
}

void SAdvanceDeletionTab::FixUpRedirectors()
{
	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>("SuperManager");
	SuperManagerModule.FixUpRedirectors();
}

void SAdvanceDeletionTab::OnDoubleClickAssetItem(TSharedPtr<FAssetData> AssetData)
{
	FSuperManagerModule::SyncCBToClickedForAssetList(AssetData->GetObjectPathString());
}

void SAdvanceDeletionTab::OnClickAssetItem(TSharedPtr<FAssetData, ESPMode::ThreadSafe> AssetData)
{
	// 依据点击的资产项，拿到该行的复选框
	if (AssetDataToCheckBoxMap.Contains(AssetData))
	{
		const TSharedPtr<SCheckBox> CheckBox = AssetDataToCheckBoxMap[AssetData];
		if (CheckBox.IsValid())
		{
			CheckBox->ToggleCheckedState();
		}
	}
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SAdvanceDeletionTab::OnConstructAssetListView()
{
	AssetDataToCheckBoxMap.Empty();

	ConstructedAssetListView = SNew(SListView<TSharedPtr<FAssetData>>)
		.ItemHeight(24)
		.ListItemsSource(&DisplayedAssetDataList)
		.OnGenerateRow(this, &SAdvanceDeletionTab::OnGenerateRowForList)
		.OnMouseButtonClick(this, &SAdvanceDeletionTab::OnClickAssetItem)
		.OnMouseButtonDoubleClick(this, &SAdvanceDeletionTab::OnDoubleClickAssetItem);

	return ConstructedAssetListView.ToSharedRef();
}

void SAdvanceDeletionTab::RefreshAssetListView()
{
	CheckBoxList.Empty();
	SelectedAssetDataList.Empty();
	AssetDataToCheckBoxMap.Empty();

	FixUpRedirectors();

	if (ConstructedAssetListView.IsValid())
	{
		// ConstructedAssetListView->RequestListRefresh();
		ConstructedAssetListView->RebuildList();
	}
	
}



#pragma region ComboBoxForListingConditions

TSharedRef<SComboBox<TSharedPtr<FString>>> SAdvanceDeletionTab::OnConstructComboBox()
{
	TSharedRef<SComboBox<TSharedPtr<FString>>> ConstructedComboBox =
		SNew(SComboBox<TSharedPtr<FString>>)
		.OptionsSource(&ComboBoxSourceItems)
		.OnGenerateWidget(this, &SAdvanceDeletionTab::OnConstructComboBoxWidget)
		.OnSelectionChanged(this, &SAdvanceDeletionTab::OnComboBoxSelectionChanged)
		[
			SAssignNew(ComboDisplayTextBlock, STextBlock)
				.Text(FText::FromString(TEXT("List Assets Option")))
		];

	return ConstructedComboBox;
}

TSharedRef<SWidget> SAdvanceDeletionTab::OnConstructComboBoxWidget(TSharedPtr<FString> Item)
{
	TSharedRef<STextBlock> TextBlock = SNew(STextBlock)
		.Text(FText::FromString(*Item.Get()));

	return TextBlock;
}

void SAdvanceDeletionTab::OnComboBoxSelectionChanged(TSharedPtr<FString> SelectedItem, ESelectInfo::Type SelectInfo)
{
	if (!SelectedItem.IsValid())
	{
		return;
	}

	ComboDisplayTextBlock->SetText(FText::FromString(*SelectedItem.Get()));

	// 更新按照选项筛选后的资产列表
	if (*SelectedItem.Get() == LIST_ALL)
	{
		DisplayedAssetDataList = AssetDataUnderSelectedFolder;

		RefreshAssetListView();
	}
	else if (*SelectedItem.Get() == LIST_UNUSED)
	{
		DisplayedAssetDataList.Empty();

		// FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>("SuperManager");
		FSuperManagerModule::GetUnusedAssets(DisplayedAssetDataList, AssetDataUnderSelectedFolder);

		RefreshAssetListView();
	}
	else if (*SelectedItem.Get() == LIST_SAME_NAME)
	{
		DisplayedAssetDataList.Empty();

		TMap<FString, int32> AssetNameMap;
		for (const TSharedPtr<FAssetData>& AssetData : AssetDataUnderSelectedFolder)
		{
			if (AssetNameMap.Contains(AssetData->AssetName.ToString()))
			{
				AssetNameMap[AssetData->AssetName.ToString()]++;
			}
			else
			{
				AssetNameMap.Add(AssetData->AssetName.ToString(), 1);
			}
		}

		for (const TSharedPtr<FAssetData>& AssetData : AssetDataUnderSelectedFolder)
		{
			if (AssetNameMap[AssetData->AssetName.ToString()] > 1)
			{
				DisplayedAssetDataList.Add(AssetData);
			}
		}

		RefreshAssetListView();
	}
}

TSharedRef<SButton> SAdvanceDeletionTab::GenerateComboBoxHelper()
{
	//// 创建一个提示图标（按钮），用户点击后会显示提示信息
	//TSharedRef<SToolTip> HelperToolTip = SNew(SToolTip)
	//	[
	//		SNew(STextBlock)
	//			.Text(FText::FromString("List All Assets: List all assets under selected folder.\nList Unused Assets: List all unused assets under selected folder.\nList Same Name Assets: List all assets with same name under selected folder."))
	//			.Justification(ETextJustify::Center)
	//			.ColorAndOpacity(FLinearColor::White)
	//	];

	TSharedRef<SButton> HelperButton = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.ButtonStyle(FAppStyle::Get(), "NoBorder") // 使用无边框样式
		.Text(FText::FromString("?"));

	HelperButton->SetToolTipText(FText::FromString("List All Assets: List all assets under selected folder.\n"
												"List Unused Assets: List all unused assets under selected folder.\n"
												"List Same Name Assets: List all assets with same name under selected folder."));

	return HelperButton;
}

#pragma endregion


#pragma region RowWidgetsForAssetListViews

TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!Item.IsValid())
	{
		return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	}

	TSharedRef<STableRow<TSharedPtr<FAssetData>>> TableRow =
	SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		[
			// 水平布局
			SNew(SHorizontalBox)

				// 第一列：复选框
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(0.05)
				.Padding(5.f)
				[
					OnGenerateCheckBox(Item)
				]

				// 第二列：资产类型（资产类名）
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(0.25)
				[
					OnGenerateTextBlockForRowWidget(Item->AssetClassPath.ToString(), FCoreStyle::Get().GetFontStyle("EmbossedText"))
				]

				// 第三列：资产名称
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(0.2)
				[
					OnGenerateTextBlockForRowWidget(Item->AssetName.ToString(), FCoreStyle::Get().GetFontStyle("EmbossedText"))
				]

				// 第四列：资产路径
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(0.4)
				[
					OnGenerateTextBlockForRowWidget(Item->GetObjectPathString(), FCoreStyle::Get().GetFontStyle("EmbossedText"))
				]

				// 第五列：资产删除按钮
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.FillWidth(0.1)
				.Padding(5.f)
				[
					OnGenerateButtonForRowWidget(Item)
				]
		];

	return TableRow;
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::OnGenerateCheckBox(const TSharedPtr<FAssetData>& Item)
{
	TSharedRef<SCheckBox> CheckBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.IsChecked(ECheckBoxState::Unchecked)
		.Visibility(EVisibility::Visible)
		.OnCheckStateChanged(this, &SAdvanceDeletionTab::OnCheckBoxStateChanged, Item);

	if (!CheckBoxList.Contains(CheckBox))
	{
		CheckBoxList.Add(CheckBox);
	}

	if (!AssetDataToCheckBoxMap.Contains(Item))
	{
		AssetDataToCheckBoxMap.Add(Item, CheckBox);
	}

	return CheckBox;
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> Item)
{
	switch (NewState)
	{
	case ECheckBoxState::Unchecked:
		{
			if (SelectedAssetDataList.Contains(Item))
			{
				SelectedAssetDataList.Remove(Item);
			}
			break;
		}
	case ECheckBoxState::Checked:
		{
			if (!SelectedAssetDataList.Contains(Item))
			{
				SelectedAssetDataList.Add(Item);
			}
			break;
		}
	case ECheckBoxState::Undetermined:		// 未确定
		if (SelectedAssetDataList.Contains(Item))
		{
			SelectedAssetDataList.Remove(Item);
		}
	default:
		break;
	}
}

TSharedRef<STextBlock> SAdvanceDeletionTab::OnGenerateTextBlockForRowWidget(const FString& TextContent, const FSlateFontInfo& FontInfo)
{
	TSharedRef<STextBlock> TextBlock = SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(FontInfo)
		.ColorAndOpacity(FLinearColor::White);

	return TextBlock;
}

FReply SAdvanceDeletionTab::OnRowDeleteButtonClicked(TSharedPtr<FAssetData> AssetData)
{
	// FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>("SuperManager");
	const bool DelRet = FSuperManagerModule::DeleteSingleAsset(*AssetData.Get());

	// 刷新表格
	if (DelRet)
	{
		if (AssetDataUnderSelectedFolder.Contains(AssetData))
		{
			AssetDataUnderSelectedFolder.Remove(AssetData);
		}

		if (DisplayedAssetDataList.Contains(AssetData))
		{
			DisplayedAssetDataList.Remove(AssetData);
		}
		
		RefreshAssetListView();
	}

	return FReply::Handled();
}

TSharedRef<SButton> SAdvanceDeletionTab::OnGenerateButtonForRowWidget(const TSharedPtr<FAssetData>& Item)
{
	TSharedRef<SButton> Button = SNew(SButton)
		.Text(FText::FromString("Delete"))
		.OnClicked(this, &SAdvanceDeletionTab::OnRowDeleteButtonClicked, Item);

	return Button;
}


#pragma endregion


#pragma region TabButtons

FReply SAdvanceDeletionTab::OnSelectAllButtonClicked()
{
	if (CheckBoxList.Num() <= 0)
	{
		return FReply::Handled();
	}

	for (const TSharedRef<SCheckBox>& CheckBox : CheckBoxList)
	{
		if (CheckBox->GetCheckedState() == ECheckBoxState::Unchecked)
		{
			CheckBox->ToggleCheckedState();
		}
	}


	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnDeselectAllButtonClicked()
{
	if (CheckBoxList.Num() <= 0)
	{
		return FReply::Handled();
	}

	for (const TSharedRef<SCheckBox>& CheckBox : CheckBoxList)
	{
		if (CheckBox->GetCheckedState() == ECheckBoxState::Checked)
		{
			CheckBox->ToggleCheckedState();
		}
	}

	return FReply::Handled();
}

FReply SAdvanceDeletionTab::OnDeleteSelectedButtonClicked()
{
	if (SelectedAssetDataList.Num() <= 0)
	{
		return FReply::Handled();
	}

	// FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>("SuperManager");
	TArray<FAssetData> SelectedAssetDataArray;
	for (const TSharedPtr<FAssetData>& AssetData : SelectedAssetDataList)
	{
		SelectedAssetDataArray.Add(*AssetData.Get());
	}

	if (FSuperManagerModule::DeleteAssets(SelectedAssetDataArray))
	{
		// 刷新表格
		for (const TSharedPtr<FAssetData>& AssetData : SelectedAssetDataList)
		{
			if (AssetDataUnderSelectedFolder.Contains(AssetData))
			{
				AssetDataUnderSelectedFolder.Remove(AssetData);
			}

			if (DisplayedAssetDataList.Contains(AssetData))
			{
				DisplayedAssetDataList.Remove(AssetData);
			}
		}

		RefreshAssetListView();
	}


	return FReply::Handled();
}

TSharedRef<SButton> SAdvanceDeletionTab::OnGenerateSelectAllButton()
{
	TSharedRef<SButton> Button = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.ButtonColorAndOpacity(FLinearColor::Green)
		.OnClicked(this, &SAdvanceDeletionTab::OnSelectAllButtonClicked);

	Button->SetContent(ConstructTextForTabButtons(FText::FromString("Select All")));

	return Button;
}

TSharedRef<SButton> SAdvanceDeletionTab::OnGenerateDeselectAllButton()
{
	TSharedRef<SButton> Button = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.ButtonColorAndOpacity(FLinearColor::Blue)
		.OnClicked(this, &SAdvanceDeletionTab::OnDeselectAllButtonClicked);

	Button->SetContent(ConstructTextForTabButtons(FText::FromString("Deselect All")));

	return Button;
}

TSharedRef<SButton> SAdvanceDeletionTab::OnGenerateDeleteSelectedButton()
{
	TSharedRef<SButton> Button = SNew(SButton)
		.ContentPadding(FMargin(5.f))
		.ButtonColorAndOpacity(FLinearColor::Red)
		.OnClicked(this, &SAdvanceDeletionTab::OnDeleteSelectedButtonClicked);

	Button->SetContent(ConstructTextForTabButtons(FText::FromString("Delete Selected")));

	return Button;
}

TSharedRef<STextBlock> SAdvanceDeletionTab::ConstructTextForTabButtons(const FText& TextContent)
{
	FSlateFontInfo FontInfo = FCoreStyle::Get().GetFontStyle("EmbossedText");
	FontInfo.Size = 16;

	TSharedRef<STextBlock> TextBlock = SNew(STextBlock)
		.Text(TextContent)
		.Font(FontInfo)
		.Justification(ETextJustify::Center)
		.ColorAndOpacity(FLinearColor::White);

	return TextBlock;
}

#pragma endregion

