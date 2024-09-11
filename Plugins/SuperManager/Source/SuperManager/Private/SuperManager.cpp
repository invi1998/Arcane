// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperManager.h"

#include "AssetToolsModule.h"
#include "DebugHeader.h"

#include "ContentBrowserModule.h"
#include "EditorAssetLibrary.h"
#include "EditorStyleSet.h"
#include "ObjectTools.h"
#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "CustomStyle/SuperManagerStyle.h"
#include "LevelEditor.h"
#include "SceneOutlinerModule.h"
#include "Engine/Selection.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "CustomUICommands/SuperManagerUICommands.h"
#include "CustomOutlinerColumn/OutlinerSelectionColumn.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

void FSuperManagerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FSuperManagerStyle::InitializeIcons();	// 初始化图标

	InitCBMenuExtender();

	RegisterAdvancedDeletionTabSpawner();

	FSuperManagerUICommands::Register();	// 注册UI命令

	InitCustomUICommands();	// 初始化自定义UI命令

	InitLevelEditorMenuExtender();

	InitCustomSelectionEvents();

	InitSceneOutlinerAlignmentExtender();	// 初始化场景大纲对齐扩展器
}

void FSuperManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(AdvancedDeletionTabIdName);	// 注销高级删除选项卡生成器

	FSuperManagerStyle::Shutdown();	// 关闭图标

	FSuperManagerUICommands::Unregister();	// 注销UI命令

	UnRegisterAlignmentColumn();	// 注销对齐列
}



#pragma region ContentBrowserMenuExtender

TSharedRef<FExtender> FSuperManagerModule::OnExtendContentBrowserPathViewMenu(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> Extender(new FExtender);

	if (SelectedPaths.Num() > 0)
	{
		SelectedFolderPaths = SelectedPaths;
		// 添加菜单项
		Extender->AddMenuExtension(
			"Delete",		// 在内容浏览器路径视图菜单中的位置
			EExtensionHook::After,		// 在Delete菜单项之后添加
			TSharedPtr<FUICommandList>(),	//
			FMenuExtensionDelegate::CreateRaw(this, &FSuperManagerModule::OnAddCBMenuEntry)	// 添加菜单项的委托方法
		);
	}

	return Extender;
}

void FSuperManagerModule::InitCBMenuExtender()
{
	FContentBrowserModule& ContentBrowser = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");	// 加载内容浏览器模块

	// 添加内容浏览器路径视图菜单扩展器
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserExtenders = ContentBrowser.GetAllPathViewContextMenuExtenders();

	// CreateRaw 创建代理对象，同时绑定委托方法（等同于先创建代理对象，再绑定委托方法）
	// CreateRaw 用于创建一个新的 TSharedRef<FContentBrowserMenuExtender_SelectedPaths> 对象，同时绑定 OnExtendContentBrowserPathViewMenu 方法
	ContentBrowserExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FSuperManagerModule::OnExtendContentBrowserPathViewMenu));

}

void FSuperManagerModule::OnAddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		LOCTEXT("Delete Unused Assets", "删除未使用的资产"),
		LOCTEXT("Safely Delete", "安全删除当前文件夹下所有未使用到的资产"),
		FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.DeleteUnusedAssets"),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnDeleteUnusedAssetsButtonClicked)
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("Delete Empty Folders", "删除空文件夹"),
		LOCTEXT("Safely Delete", "安全删除当前文件夹下所有空文件夹"),
		FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.DeleteEmptyFolders"),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnDeleteEmptyFoldersButtonClicked)
	);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("Advance Deletion", "高级删除"),
		LOCTEXT("List assets by specific conditions in a table and delete them", "按照特定条件在表格中列出资产并删除"),
		FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.AdvanceDeletion"),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnAdvanceDeletionButtonClicked)
	);

}

void FSuperManagerModule::OnDeleteUnusedAssetsButtonClicked()
{
	const TArray<FString> TempFoldersArray = SelectedFolderPaths;	// 临时文件夹数组
	if (AdvancedDeletionTab.IsValid())
	{
		AdvancedDeletionTab->RequestCloseTab();
		SelectedFolderPaths = TempFoldersArray;
		// SM_Debug::ShowMessageDialog(FText::FromString("Please close the advanced deletion tab first"), FText::FromString("Warning"), EAppMsgType::Ok);
		// return;
	}

	// 删除未使用的资产按钮点击事件
	if (SelectedFolderPaths.Num() > 1)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("You can only select one folder at a time"), FText::FromString("Warning"), EAppMsgType::Ok);
		return;
	}

	const TArray<FString> AssetPathsName = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0], true);	// 获取文件夹下所有资产路径
	if (AssetPathsName.Num() == 0)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("No assets found in the selected folder"), FText::FromString("Warning"), EAppMsgType::Ok);
		return;
	}

	const EAppReturnType::Type Result = SM_Debug::ShowMessageDialog(FText::FromString("Are you sure you want to delete all unused assets in the selected folder?"), FText::FromString("Warning"), EAppMsgType::YesNo);

	if (Result == EAppReturnType::No)
	{
		return;
	}

	FixUpRedirectors();	// 修复重定向器

	TArray<FAssetData> UnusedAssetsData;	// 未使用的资产数据
	for (const FString& AssetPath : AssetPathsName)
	{
		// 不要处理根路径下的 Developers 和 Collections 文件夹，因为这些文件夹通常是引擎或插件的文件夹，虽然不会出现在内容浏览器中，但是我们还是要做一下处理
		// 删除这两个文件夹下的内容会导致引擎或插件无法正常运行（崩溃）
		if (AssetPath.Contains(TEXT("Developers")) || AssetPath.Contains(TEXT("	Collections")) || AssetPath.Contains(TEXT("__ExternalActors__")) || AssetPath.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPath))
		{
			continue;
		}

		TArray<FString> AssetReferences = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPath);	// 获取资源引用者

		if (AssetReferences.Num() == 0)
		{
			const FAssetData AssetData = UEditorAssetLibrary::FindAssetData(AssetPath);	// 获取资产数据
			UnusedAssetsData.Add(AssetData);
		}
	}

	if (UnusedAssetsData.Num() > 0)
	{
		ObjectTools::DeleteAssets(UnusedAssetsData, true);	// 删除未使用的资产

		SM_Debug::ShowNotifyInfo(FText::FromString(FString::Printf(TEXT("Deleted %d unused assets"), UnusedAssetsData.Num())), FText::FromString("Success"));

		const EAppReturnType::Type DeleteEmptyFolderConfirmResult = SM_Debug::ShowMessageDialog(FText::FromString("Would you like to delete empty folders in the selected folder?"), FText::FromString("Warning"), EAppMsgType::YesNo);

		if (DeleteEmptyFolderConfirmResult == EAppReturnType::Yes)
		{
			OnDeleteEmptyFoldersButtonClicked();
		}
	}
	else
	{
		SM_Debug::ShowMessageDialog(FText::FromString("No unused assets found in the selected folder"), FText::FromString("Warning"), EAppMsgType::Ok);
	}

}

void FSuperManagerModule::OnDeleteEmptyFoldersButtonClicked()
{
	const TArray<FString> TempFoldersArray = SelectedFolderPaths;	// 临时文件夹数组
	if (AdvancedDeletionTab.IsValid())
	{
		AdvancedDeletionTab->RequestCloseTab();
		SelectedFolderPaths = TempFoldersArray;
		// SM_Debug::ShowMessageDialog(FText::FromString("Please close the advanced deletion tab first"), FText::FromString("Warning"), EAppMsgType::Ok);
		// return;
	}

	FixUpRedirectors();	// 修复重定向器

	if (SelectedFolderPaths.Num() > 1)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("You can only select one folder at a time"), FText::FromString("Warning"), EAppMsgType::Ok);
		return;
	}

	if (SelectedFolderPaths.Num() == 0)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("No folder selected"), FText::FromString("Warning"), EAppMsgType::Ok);
		return;
	}

	// ListAssets第二个参数为true时，会递归获取所有子目录下的资产，第三个参数为true时，会递归获取所有子目录下的文件夹
	TArray<FString> FoldersPathsArray = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0], true, true);	// 获取文件夹路径

	TArray<FString> EmptyFoldersArray;	// 空文件夹数组
	FString EmptyFolderName;	// 空文件夹名称

	for (const FString& FolderPath : FoldersPathsArray)
	{
		if (FolderPath.Contains(TEXT("Developers")) || FolderPath.Contains(TEXT("	Collections")) || FolderPath.Contains(TEXT("__ExternalActors__")) || FolderPath.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesDirectoryExist(FolderPath))
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath))
		{
			EmptyFolderName.Append(FolderPath);
			EmptyFolderName.Append(TEXT("\n"));

			EmptyFoldersArray.Add(FolderPath);
		}
	}

	if (EmptyFoldersArray.Num() > 0)
	{
		uint32 DeletedFoldersCount = 0;
		const EAppReturnType::Type Result = SM_Debug::ShowMessageDialog(FText::FromString(FString::Printf(TEXT("Are you sure you want to delete all empty folders (%d) in the selected folder?\n\n%s"), EmptyFoldersArray.Num(), *EmptyFolderName)), FText::FromString("Warning"), EAppMsgType::OkCancel);

		if (Result == EAppReturnType::Cancel)
		{
			return;
		}

		for (const FString& EmptyFolder : EmptyFoldersArray)
		{
			UEditorAssetLibrary::DeleteDirectory(EmptyFolder) ? DeletedFoldersCount++ : SM_Debug::ShowMessageDialog(FText::FromString(FString::Printf(TEXT("Failed to delete folder: %s"), *EmptyFolder)), FText::FromString("Error"), EAppMsgType::Ok);
		}

		SM_Debug::ShowNotifyInfo(FText::FromString(FString::Printf(TEXT("Deleted %d empty folders"), DeletedFoldersCount)), FText::FromString("Success"));
	}
	else
	{
		SM_Debug::ShowMessageDialog(FText::FromString("No empty folders found in the selected folder"), FText::FromString("Warning"), EAppMsgType::Ok);
	}
}

void FSuperManagerModule::FixUpRedirectors()
{
	TArray<UObjectRedirector*> RedirectorToFixArray;	// 重定向器数组

	// 获取资产注册模块
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	// 创建过滤器
	FARFilter Filter;
	Filter.bRecursivePaths = true;	// 递归路径，即包含子目录
	Filter.PackagePaths.Emplace("/Game");	// 添加包路径

	// 类路径名的筛选器组件。将包含指定类的实例，但不包括子类（默认情况下）的实例。仅当 bRecursiveClasses 为 true 时，才会包含派生类。
	Filter.ClassPaths.Emplace(USkeletalMeshComponent::StaticClass()->GetPathName());	// 添加类路径

	// 5.1 之后的版本已被弃用（改为使用ClassPaths）
	// Filter.ClassNames.Emplace(USkeletalMeshComponent::StaticClass()->GetFName());	// 添加类名

	TArray<FAssetData> OutRedirectorAssets;

	AssetRegistryModule.Get().GetAssets(Filter, OutRedirectorAssets);		// 获取资源

	for (const FAssetData& AssetData : OutRedirectorAssets)
	{
		if (UObject* Asset = AssetData.GetAsset())	// 获取资源
		{
			if (UObjectRedirector* Redirector = Cast<UObjectRedirector>(Asset))	// 转换为重定向器
			{
				RedirectorToFixArray.Add(Redirector);	// 添加重定向器 
			}
		}
	}

	// 修复重定向器
	FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get().FixupReferencers(RedirectorToFixArray);
}

void FSuperManagerModule::OnAdvanceDeletionButtonClicked()
{
	const TArray<FString> TempFoldersArray = SelectedFolderPaths;	// 临时文件夹数组
	if (AdvancedDeletionTab.IsValid())
	{
		AdvancedDeletionTab->RequestCloseTab();
		SelectedFolderPaths = TempFoldersArray;
		// SM_Debug::ShowMessageDialog(FText::FromString("Please close the advanced deletion tab first"), FText::FromString("Warning"), EAppMsgType::Ok);
		// return;
	}

	// 高级删除按钮点击事件
	FGlobalTabmanager::Get()->TryInvokeTab(AdvancedDeletionTabIdName);
}

#pragma endregion


#pragma region CustomEditorTabs

void FSuperManagerModule::RegisterAdvancedDeletionTabSpawner()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(AdvancedDeletionTabIdName, FOnSpawnTab::CreateRaw(this, &FSuperManagerModule::OnSpawnAdvancedDeletionTab))
	.SetDisplayName(LOCTEXT("AdvancedDeletionTab", "高级删除"));
}

void FSuperManagerModule::OnAdvancedDeletionTabClosed(TSharedRef<SDockTab> ClosedTab)
{
	// 高级删除选项卡关闭事件
	if (AdvancedDeletionTab.IsValid())
	{
		AdvancedDeletionTab.Reset();
		SelectedFolderPaths.Empty();
	}
}

TSharedRef<SDockTab> FSuperManagerModule::OnSpawnAdvancedDeletionTab(const FSpawnTabArgs& Args)
{
	// 生成高级删除选项卡
	if (SelectedFolderPaths.Num() == 0)
	{
		return SNew(SDockTab).TabRole(ETabRole::NomadTab);
	}

	AdvancedDeletionTab = SNew(SDockTab).TabRole(ETabRole::NomadTab)
		[
			SNew(SAdvanceDeletionTab)
				.AssetDataList(GetAllAssetDatasUnderSelectedFolder())
				.CurrentSelectedFolderPath(SelectedFolderPaths.Num() > 0 ? SelectedFolderPaths[0] : FString())
		];

	// 设置选项卡关闭事件
	AdvancedDeletionTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateRaw(this, &FSuperManagerModule::OnAdvancedDeletionTabClosed));

	return AdvancedDeletionTab.ToSharedRef();
}

TArray<TSharedPtr<FAssetData>> FSuperManagerModule::GetAllAssetDatasUnderSelectedFolder()
{
	FixUpRedirectors();

	// 获取此文件夹下的所有资产数据
	TArray<TSharedPtr<FAssetData>> AssetDataList;

	if (SelectedFolderPaths.Num() > 1)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("You can only select one folder at a time"), FText::FromString("Warning"), EAppMsgType::Ok);
		return AssetDataList;
	}

	const TArray<FString> AssetPathsName = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0], true);	// 获取文件夹下所有资产路径
	if (AssetPathsName.Num() == 0)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("No assets found in the selected folder"), FText::FromString("Warning"), EAppMsgType::Ok);
		return AssetDataList;
	}

	for (const FString& AssetPath : AssetPathsName)
	{
		if (AssetPath.Contains(TEXT("Developers")) || AssetPath.Contains(TEXT("	Collections")) || AssetPath.Contains(TEXT("__ExternalActors__")) || AssetPath.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPath))
		{
			continue;
		}

		const FAssetData AssetData = UEditorAssetLibrary::FindAssetData(AssetPath);	// 获取资产数据
		AssetDataList.Add(MakeShared<FAssetData>(AssetData));
	}

	return AssetDataList;
}




#pragma endregion


#pragma region LevelEditorMenuExtender

void FSuperManagerModule::OnLockActorSelectionButtonClicked()
{
	if (!GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = WeakEditorActorSubsystem->GetSelectedLevelActors();	// 获取选中的Actor

	FString SelectedActorsName = TEXT("Selected actors: ");
	for (AActor* SelectedActor : SelectedActors)
	{
		// 锁定Actor选择
		LockActorSelection(SelectedActor);

		WeakEditorActorSubsystem->SetActorSelectionState(SelectedActor, false);	// 取消选择

		SelectedActorsName.Append("\n" + SelectedActor->GetActorLabel());

	}
	RefreshSceneOutliner();	// 刷新场景大纲

	SelectedActorsName.Append("\n Have been locked");
	SM_Debug::ShowNotifyInfo(FText::FromString(SelectedActorsName), FText::FromString("Success"));
}

void FSuperManagerModule::OnUnlockActorSelectionButtonClicked()
{
	if (!GetEditorActorSubsystem()) return;

	TArray<AActor*> AllActors = WeakEditorActorSubsystem->GetAllLevelActors();	// 获取所有的Actor

	FString UnlockedActorsName = TEXT("These actors: ");

	for (AActor* Actor : AllActors)
	{
		if (IsActorSelectionLocked(Actor))
		{
			// 解锁Actor选择
			UnlockActorSelection(Actor);

			WeakEditorActorSubsystem->SetActorSelectionState(Actor, true);

			UnlockedActorsName.Append("\n" + Actor->GetActorLabel());
		}
	}

	RefreshSceneOutliner();	// 刷新场景大纲

	UnlockedActorsName.Append("\n Have been unlocked");

	SM_Debug::ShowNotifyInfo(FText::FromString(UnlockedActorsName), FText::FromString("Success"));

}

void FSuperManagerModule::OnAddLevelEditorMenuEntry(FMenuBuilder& MenuBuilder)
{
	// 添加关卡编辑器菜单项
	MenuBuilder.AddMenuEntry(
		LOCTEXT("Lock Actor Selection", "锁定Actor选择"),
		LOCTEXT("Prevent actor from being deselected", "防止Actor被取消选择"),
		FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.SelectionLock"),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnLockActorSelectionButtonClicked));

	MenuBuilder.AddMenuEntry(
		LOCTEXT("Unlock All Actors Selection", "解锁所有Actor选择"),
		LOCTEXT("Remove the selectin constraint of all actors", "移除所有Actor的选择约束"),
		FSlateIcon(FSuperManagerStyle::GetStyleSetName(), "ContentBrowser.SelectionUnlock"),
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnUnlockActorSelectionButtonClicked));
}


TSharedRef<FExtender> FSuperManagerModule::CustomLevelEditorMenuExtender(const TSharedRef<FUICommandList> UICommandList, const TArray<AActor*> SelectedActors)
{
	
	// 自定义关卡编辑器菜单扩展器
	TSharedRef<FExtender> MenuExtender = MakeShareable(new FExtender);

	if (SelectedActors.Num() > 0)
	{
		MenuExtender->AddMenuExtension(
			"ActorOptions",
			EExtensionHook::After,
			UICommandList,
			FMenuExtensionDelegate::CreateRaw(this, &FSuperManagerModule::OnAddLevelEditorMenuEntry));
	}


	return MenuExtender;
}

void FSuperManagerModule::InitLevelEditorMenuExtender()
{
	// 初始化关卡编辑器菜单扩展器
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	const TSharedRef<FUICommandList> ExistingLevelEditorCommands = LevelEditorModule.GetGlobalLevelEditorActions();	// 获取全局关卡编辑器命令列表
	ExistingLevelEditorCommands->Append(CustomUICommandList.ToSharedRef());	// 添加自定义UI命令列表

	// 获取所有关卡视口上下文菜单扩展器
	TArray<FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors>& LevelEditorMenuExtenders = LevelEditorModule.GetAllLevelViewportContextMenuExtenders();

	LevelEditorMenuExtenders.Add(FLevelEditorModule::FLevelViewportMenuExtender_SelectedActors::CreateRaw(this, &FSuperManagerModule::CustomLevelEditorMenuExtender));
}


#pragma endregion

#pragma region SelectionLock

void FSuperManagerModule::OnActorSelected(UObject* SelectedObject)
{
	if (!GetEditorActorSubsystem()) return;

	// Actor被选中
	if (AActor* SelectedActor = Cast<AActor>(SelectedObject))
	{
		if (IsActorSelectionLocked(SelectedActor))
		{
			// 如果Actor选择被锁定，则取消选择
			WeakEditorActorSubsystem->SetActorSelectionState(SelectedActor, false);
		}
	}
}

void FSuperManagerModule::InitCustomSelectionEvents()
{
	// 我们可以通过绑定事件来实现自定义选择事件
	USelection* UserSelection = GEditor->GetSelectedActors();	// 获取选中的Actor
	UserSelection->SelectObjectEvent.AddRaw(this, &FSuperManagerModule::OnActorSelected);	// 选中Actor事件
}

void FSuperManagerModule::LockActorSelection(AActor* Actor)
{
	if (Actor)
	{
		// 锁定Actor选择
		if (!Actor->Tags.Contains(TEXT("Locked")))
		{
			Actor->Tags.Add(TEXT("Locked"));
		}
	}
}

void FSuperManagerModule::UnlockActorSelection(AActor* Actor)
{
	if (Actor)
	{
		// 解锁Actor选择
		if (Actor->Tags.Contains(TEXT("Locked")))
		{
			Actor->Tags.Remove(TEXT("Locked"));
		}
	}
}

void FSuperManagerModule::RefreshSceneOutliner()
{
	// 刷新场景大纲
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	TSharedPtr<ISceneOutliner> SceneOutliner = LevelEditorModule.GetFirstLevelEditor()->GetMostRecentlyUsedSceneOutliner();
	if (SceneOutliner.IsValid())
	{
		SceneOutliner->FullRefresh();
	}
}

bool FSuperManagerModule::IsActorSelectionLocked(const AActor* Actor)
{
	if (Actor)
	{
		// Actor选择是否被锁定
		return Actor->Tags.Contains(TEXT("Locked"));
	}

	return false;

}

void FSuperManagerModule::ProcessActorSelectionLock(AActor* Actor, bool bLock)
{
	if (!GetEditorActorSubsystem()) return;

	if (bLock)
	{
		LockActorSelection(Actor);

		WeakEditorActorSubsystem->SetActorSelectionState(Actor, false);
	}
	else
	{
		UnlockActorSelection(Actor);

		WeakEditorActorSubsystem->SetActorSelectionState(Actor, true);
	}
}


#pragma endregion


#pragma region UICommandHotKeyLock

void FSuperManagerModule::InitCustomUICommands()
{
	// 初始化自定义UI命令
	CustomUICommandList = MakeShareable(new FUICommandList);

	// 添加UI命令
	CustomUICommandList->MapAction(
		FSuperManagerUICommands::Get().LockActorSelection,
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnSelectionLockHotKeyPressed)
	);

	CustomUICommandList->MapAction(
		FSuperManagerUICommands::Get().UnlockActorSelection,
		FExecuteAction::CreateRaw(this, &FSuperManagerModule::OnSelectionUnlockHotKeyPressed)
	);
}

void FSuperManagerModule::OnSelectionLockHotKeyPressed()
{
	// 选择锁定热键按下
	OnLockActorSelectionButtonClicked();
}

void FSuperManagerModule::OnSelectionUnlockHotKeyPressed()
{
	// 选择解锁热键按下
	OnUnlockActorSelectionButtonClicked();
}


#pragma endregion


#pragma region SceneOutlinerAlignmentExtender

void FSuperManagerModule::InitSceneOutlinerAlignmentExtender()
{
	FSceneOutlinerModule& SceneOutlinerModule = FModuleManager::LoadModuleChecked<FSceneOutlinerModule>("SceneOutliner");

	FSceneOutlinerColumnInfo SelectionLockColumnInfo(
		ESceneOutlinerColumnVisibility::Visible,	// 列可见性
		1,											// 列索引
		FCreateSceneOutlinerColumn::CreateRaw(this, &FSuperManagerModule::OnCreateAlignmentColumn)	// 创建场景大纲列的委托方法
	);

	SceneOutlinerModule.RegisterDefaultColumnType<FOutlinerSelectionColumn>(SelectionLockColumnInfo);		// 注册默认列类型
}

TSharedRef<ISceneOutlinerColumn> FSuperManagerModule::OnCreateAlignmentColumn(ISceneOutliner& SceneOutliner)
{
	// 创建对齐列
	TSharedRef<ISceneOutlinerColumn> Column = MakeShared<FOutlinerSelectionColumn>(SceneOutliner);
	return Column;
}

void FSuperManagerModule::UnRegisterAlignmentColumn()
{
	FSceneOutlinerModule& SceneOutlinerModule = FModuleManager::LoadModuleChecked<FSceneOutlinerModule>("SceneOutliner");

	SceneOutlinerModule.UnRegisterColumnType<FOutlinerSelectionColumn>();
}

#pragma endregion


bool FSuperManagerModule::GetEditorActorSubsystem()
{
	// 获取编辑器Actor子系统
	if (!WeakEditorActorSubsystem.IsValid())
	{
		WeakEditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	}

	return WeakEditorActorSubsystem.IsValid();
}


#pragma region ProccessDataForAssetList


bool FSuperManagerModule::DeleteSingleAsset(const FAssetData& AssetData)
{
	// 删除单个资产
	TArray<FAssetData> AssetDataArray;
	AssetDataArray.Add(AssetData);

	return ObjectTools::DeleteAssets(AssetDataArray, true) > 0;
}

bool FSuperManagerModule::DeleteAssets(const TArray<FAssetData>& AssetDatas)
{
	// 删除资产
	return ObjectTools::DeleteAssets(AssetDatas, true) > 0;
}

void FSuperManagerModule::GetUnusedAssets(TArray<TSharedPtr<FAssetData>>& OutArray, const TArray<TSharedPtr<FAssetData>>& AssetDatas)
{
	// 获取未使用的资产
	for (const TSharedPtr<FAssetData>& AssetData : AssetDatas)
	{
		TArray<FString> AssetReferences = UEditorAssetLibrary::FindPackageReferencersForAsset(AssetData->GetObjectPathString());	// 获取资源引用者

		if (AssetReferences.Num() == 0)
		{
			OutArray.Add(AssetData);
		}
	}
}

void FSuperManagerModule::SyncCBToClickedForAssetList(const FString& AssetPath)
{
	// 同步内容浏览器到点击的资产列表
	TArray<FString> AssetPaths;
	AssetPaths.Add(AssetPath);

	UEditorAssetLibrary::SyncBrowserToObjects(AssetPaths);
}

#pragma endregion



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSuperManagerModule, SuperManager)