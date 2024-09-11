// Copyright INVI_1998


#include "CustomOutlinerColumn/OutlinerSelectionColumn.h"
#include "CustomStyle/SuperManagerStyle.h"
#include "ActorTreeItem.h"
#include "SuperManager.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

FOutlinerSelectionColumn::FOutlinerSelectionColumn(ISceneOutliner& SceneOutliner)
{
}

FName FOutlinerSelectionColumn::GetColumnID()
{
	return "SelectionLock";
}

FName FOutlinerSelectionColumn::GetID()
{
	return "SelectionLock";
}

SHeaderRow::FColumn::FArguments FOutlinerSelectionColumn::ConstructHeaderRowColumn()
{
	SHeaderRow::FColumn::FArguments Args = SHeaderRow::Column(GetColumnID())
		.HAlignHeader(HAlign_Center)
		.VAlignHeader(VAlign_Center)
		.HAlignCell(HAlign_Center)
		.VAlignCell(VAlign_Center)
		.FixedWidth(24.0f)
		.DefaultTooltip(LOCTEXT("Actor Selection Lock - Press icon to lock or unlock actor selection", "Acotr锁定选择 - 点击图标锁定或解锁Actor选择"))
		[
			SNew(SImage)
				.ColorAndOpacity(FSlateColor::UseForeground())
				.Image(FSuperManagerStyle::GetSlateStyleSet()->GetBrush("ContentBrowser.SelectionLockNew"))
		];

	return Args;
}

const TSharedRef<SWidget> FOutlinerSelectionColumn::ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row)
{
	const FActorTreeItem* ActorTreeItem = TreeItem->CastTo<FActorTreeItem>();

	if (!ActorTreeItem || !ActorTreeItem->IsValid())
	{
		return SNullWidget::NullWidget;
	}

	FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>("SuperManager");

	bool bIsLocked = SuperManagerModule.IsActorSelectionLocked(ActorTreeItem->Actor.Get());

	const FCheckBoxStyle& CheckBoxStyle = FSuperManagerStyle::GetSlateStyleSet()->GetWidgetStyle<FCheckBoxStyle>("SelectionLockCheckBox");

	TSharedRef<SCheckBox> RowWidgetCheckBox = SNew(SCheckBox)
		.Visibility(EVisibility::Visible)
		.Type(ESlateCheckBoxType::ToggleButton)
		.Style(&CheckBoxStyle)		// 使用自定义样式 == Style(FSuperManagerStyle::GetSlateStyleSet(), "SelectionLockCheckBox")
		.HAlign(HAlign_Center)
		.IsChecked(bIsLocked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
		.OnCheckStateChanged(this, &FOutlinerSelectionColumn::OnRowCheckBoxStateChanged, ActorTreeItem->Actor);

	return RowWidgetCheckBox;
}

void FOutlinerSelectionColumn::OnRowCheckBoxStateChanged(ECheckBoxState NewState, TWeakObjectPtr<AActor> CorrespondingActor)
{
	if (CorrespondingActor.IsValid())
	{
		FSuperManagerModule& SuperManagerModule = FModuleManager::LoadModuleChecked<FSuperManagerModule>("SuperManager");

		if (NewState == ECheckBoxState::Checked)
		{
			SuperManagerModule.ProcessActorSelectionLock(CorrespondingActor.Get(), true);
		}
		else
		{
			SuperManagerModule.ProcessActorSelectionLock(CorrespondingActor.Get(), false);
		}
	}
}

#undef LOCTEXT_NAMESPACE
