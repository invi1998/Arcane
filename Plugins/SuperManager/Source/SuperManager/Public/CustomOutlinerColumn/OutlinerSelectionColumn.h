// Copyright INVI_1998

#pragma once

#include "ISceneOutlinerColumn.h"

class FOutlinerSelectionColumn : public ISceneOutlinerColumn
{
public:
	FOutlinerSelectionColumn(ISceneOutliner& SceneOutliner);

	virtual FName GetColumnID() override;

	static FName GetID();

	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;

	virtual const TSharedRef<SWidget> ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row) override;

private:
	void OnRowCheckBoxStateChanged(ECheckBoxState NewState, TWeakObjectPtr<AActor> CorrespondingActor);	// 行复选框状态改变事件

};

