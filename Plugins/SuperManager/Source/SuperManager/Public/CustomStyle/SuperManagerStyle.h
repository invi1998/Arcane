// Copyright INVI_1998

#pragma once

#include "Styling/SlateStyle.h"

class FSuperManagerStyle
{

public:
	static void InitializeIcons();

	static void Shutdown();

	static FName GetStyleSetName()
	{
		return StyleSetName;
	}

	static TSharedRef<FSlateStyleSet> GetSlateStyleSet();

private:
	static FName StyleSetName;

	static TSharedRef<FSlateStyleSet> CreateSlateStyleSet();
	static TSharedPtr<FSlateStyleSet> CustomSlateStyleSet;
	
};
