// Copyright INVI_1998


#include "CustomUICommands/SuperManagerUICommands.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

FSuperManagerUICommands::FSuperManagerUICommands()
	: TCommands<FSuperManagerUICommands>(
		"SuperManager",
		LOCTEXT("SuperManagerUICommands", "超级管理器UI热键面板"),
		NAME_None,
		"SuperManager")
{
}

void FSuperManagerUICommands::RegisterCommands()
{
	UI_COMMAND(
		LockActorSelection,
		"Lock Actor Selection",
		"Lock actor selection in level, onece triggered, the actor will be locked and can't be selected",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::W, EModifierKey::Alt)	// Alt + W
	);

	UI_COMMAND(
		UnlockActorSelection,
		"Unlock Actor Selection",
		"Remove selection lock on all actors in level, onece triggered, the actor will be unlocked and can be selected",
		EUserInterfaceActionType::Button,
		FInputChord(EKeys::W, EModifierKey::Alt | EModifierKey::Shift)	// Alt + Shift + W
	);
}

#undef LOCTEXT_NAMESPACE
