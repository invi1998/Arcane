// Copyright INVI_1998


#include "CustomStyle/SuperManagerStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateColor.h"

FName FSuperManagerStyle::StyleSetName = TEXT("SuperManagerStyle");
TSharedPtr<FSlateStyleSet> FSuperManagerStyle::CustomSlateStyleSet = nullptr;

void FSuperManagerStyle::InitializeIcons()
{
	if (!CustomSlateStyleSet.IsValid())
	{
		CustomSlateStyleSet = CreateSlateStyleSet();
	}

	if (!FSlateStyleRegistry::FindSlateStyle(StyleSetName))
	{
		FSlateStyleRegistry::RegisterSlateStyle(*CustomSlateStyleSet);
	}
}

void FSuperManagerStyle::Shutdown()
{
	if (CustomSlateStyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*CustomSlateStyleSet);
		CustomSlateStyleSet.Reset();
	}
}

TSharedRef<FSlateStyleSet> FSuperManagerStyle::GetSlateStyleSet()
{
	return CustomSlateStyleSet.ToSharedRef();
}

TSharedRef<FSlateStyleSet> FSuperManagerStyle::CreateSlateStyleSet()
{
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet(StyleSetName));

	IPluginManager& PluginManager = IPluginManager::Get();

	const FString IconDir = PluginManager.FindPlugin(TEXT("SuperManager"))->GetBaseDir() / TEXT("Resources");

	Style->Set("ContentBrowser.DeleteUnusedAssets", new FSlateImageBrush(IconDir / TEXT("DeleteUnusedAsset.png"), FVector2D(16.0f, 16.0f)));
	Style->Set("ContentBrowser.DeleteEmptyFolders", new FSlateImageBrush(IconDir / TEXT("DeleteEmptyFolders.png"), FVector2D(16.0f, 16.0f)));
	Style->Set("ContentBrowser.AdvanceDeletion", new FSlateImageBrush(IconDir / TEXT("AdvanceDeletion.png"), FVector2D(16.0f, 16.0f)));
	Style->Set("ContentBrowser.SelectionLock", new FSlateImageBrush(IconDir / TEXT("SelectionLock.png"), FVector2D(16.0f, 16.0f)));
	Style->Set("ContentBrowser.SelectionUnlock", new FSlateImageBrush(IconDir / TEXT("SelectionUnlock.png"), FVector2D(16.0f, 16.0f)));
	Style->Set("ContentBrowser.SelectionLockNew", new FSlateImageBrush(IconDir / TEXT("SelectionLockNew.png"), FVector2D(16.0f, 16.0f)));


	const FCheckBoxStyle SelectionLockCheckBoxStyle = FCheckBoxStyle()
		.SetCheckBoxType(ESlateCheckBoxType::ToggleButton)
		.SetPadding(FMargin(10.0f))

		// 未选中状态下的背景图
		.SetUncheckedImage(FSlateImageBrush(IconDir / TEXT("SelectionLockNew.png"), FVector2D(16.0f, 16.0f), FStyleColors::White))
		.SetUncheckedHoveredImage(FSlateImageBrush(IconDir / TEXT("SelectionLockNew.png"), FVector2D(16.0f, 16.0f), FStyleColors::AccentBlue))
		.SetUncheckedPressedImage(FSlateImageBrush(IconDir / TEXT("SelectionLockNew.png"), FVector2D(16.0f, 16.0f), FStyleColors::Foreground))

		// 选中状态下的背景图
		.SetCheckedImage(FSlateImageBrush(IconDir / TEXT("SelectionLockNew.png"), FVector2D(16.0f, 16.0f), FStyleColors::Foreground))
		.SetCheckedHoveredImage(FSlateImageBrush(IconDir / TEXT("SelectionLockNew.png"), FVector2D(16.0f, 16.0f), FStyleColors::AccentBlack))
		.SetCheckedPressedImage(FSlateImageBrush(IconDir / TEXT("SelectionLockNew.png"), FVector2D(16.0f, 16.0f), FStyleColors::AccentGray));

	Style->Set("SelectionLockCheckBox", SelectionLockCheckBoxStyle);

	return Style;
}
