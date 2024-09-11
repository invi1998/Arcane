// Copyright INVI_1998

#pragma once

#include "Framework/Commands/Commands.h"

// UI
class FSuperManagerUICommands : public TCommands<FSuperManagerUICommands>
{

public:

	FSuperManagerUICommands();

	// TCommand<> interface
	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> LockActorSelection;		// 当前选择的Actor锁定
	TSharedPtr<FUICommandInfo> UnlockActorSelection;	// 当前选择的Actor解锁
};
