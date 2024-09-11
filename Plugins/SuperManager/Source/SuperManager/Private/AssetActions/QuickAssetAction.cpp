// Copyright INVI_1998


#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"

void UQuickAssetAction::DuplicateSelectedAssets(int32 NumCopies) const
{
	if (NumCopies <= 0)
	{
		// 使用消息框显示错误信息
		SM_Debug::ShowMessageDialog(FText::FromString("Invalid number of copies"), FText::FromString("Error"), EAppMsgType::Ok);

		return;
	}

	FixUpRedirectors();	// 修复重定向器

	TArray<FAssetData> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssetData();	// 获取选中的资源
	if (SelectedAssets.Num() == 0)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("No assets selected"), FText::FromString("Warning"), EAppMsgType::Ok);

		return;
	}

	uint32 NumDuplicatedAssets = 0;

	for (const FAssetData& Asset : SelectedAssets)
	{
		for (int32 i = 0; i < NumCopies; ++i)
		{
			FString SourceAssetPath = Asset.GetSoftObjectPath().ToString();		// 获取资源路径
			FString NewAssetName = Asset.AssetName.ToString() + FString::Printf(TEXT("_Copy%d"), i+1);	// 新资源名称
			FString NewAssetPathName = FPaths::Combine(Asset.PackagePath.ToString(), NewAssetName);	// 新资源路径

			FAssetData NewAsset = UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewAssetPathName);	// 复制资源
			if (NewAsset.IsValid())
			{
				UEditorAssetLibrary::SaveAsset(NewAssetPathName, false);	// 保存资源
				++NumDuplicatedAssets;

				SM_Debug::PrintDebug(FString::Printf(TEXT("Duplicated asset: %s"), *NewAsset.AssetName.ToString()), FColor::Green);
				SM_Debug::PrintLog(FString::Printf(TEXT("Duplicated asset: %s"), *NewAsset.AssetName.ToString()));
			}
			else
			{
				SM_Debug::PrintDebug(FString::Printf(TEXT("Failed to duplicate asset: %s"), *Asset.AssetName.ToString()), FColor::Red);
				SM_Debug::PrintLog(FString::Printf(TEXT("Failed to duplicate asset: %s"), *Asset.AssetName.ToString()));
			}
		}
	}

	if (NumDuplicatedAssets > 0)
	{
		SM_Debug::ShowNotifyInfo(FText::FromString(FString::Printf(TEXT("Duplicated %d assets"), NumDuplicatedAssets)), FText::FromString("Success"));
	}
	else
	{
		SM_Debug::ShowNotifyInfo(FText::FromString("No assets duplicated"), FText::FromString("Warning"));
	}
}

void UQuickAssetAction::AddPrefixToSelectedAssets() const
{
	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();	// 获取选中的资源
	uint32 NumPrefixedAssets = 0;

	for (UObject* Asset : SelectedAssets)
	{
		if (!Asset)
		{
			SM_Debug::PrintDebug(FString::Printf(TEXT("Invalid asset: %s"), *Asset->GetName()), FColor::Red);
			SM_Debug::PrintLog(FString::Printf(TEXT("Invalid asset: %s"), *Asset->GetName()));

			continue;
		}

		FString Prefix = AssetPrefixMap.FindRef(Asset->GetClass());	// 获取前缀
		if (Prefix.IsEmpty())
		{
			// 打印Asset->GetClass()的名称
			SM_Debug::PrintDebug(FString::Printf(TEXT("No prefix for asset: %s"), *Asset->GetName()), FColor::Red);
			SM_Debug::PrintLog(FString::Printf(TEXT("No prefix for asset: %s"), *Asset->GetName()));

			continue;
		}

		FString OldAssetName = Asset->GetName();	// 旧资源名称

		// 如果资源名称已经包含前缀，则跳过
		if (OldAssetName.StartsWith(Prefix))
		{
			SM_Debug::PrintDebug(FString::Printf(TEXT("Asset already prefixed: %s"), *OldAssetName), FColor::Yellow);
			SM_Debug::PrintLog(FString::Printf(TEXT("Asset already prefixed: %s"), *OldAssetName));

			continue;
		}

		// 如果老的资源名称包含前缀，则去掉前缀
		for (const auto& PrefixPair : AssetPrefixMap)
		{
			if (OldAssetName.StartsWith(PrefixPair.Value))
			{
				OldAssetName = OldAssetName.RightChop(PrefixPair.Value.Len());		// 去掉前缀
				SM_Debug::PrintDebug(FString::Printf(TEXT("Removed prefix %s from asset: %s"), *PrefixPair.Value, *OldAssetName), FColor::Yellow);
				break;
			}
		}

		// 如果是材质实例，需要去掉后缀（_Inst）
		if (Asset->IsA<UMaterialInstanceConstant>())
		{
			OldAssetName.RemoveFromEnd(TEXT("_Inst"));
		}

		FString NewAssetName = Prefix + OldAssetName;	// 新资源名称

		UEditorUtilityLibrary::RenameAsset(Asset, NewAssetName);	// 重命名资源
		++NumPrefixedAssets;
	}

	SM_Debug::ShowNotifyInfo(FText::FromString(FString::Printf(TEXT("Prefixed %d assets"), NumPrefixedAssets)), FText::FromString("Success"));
}

void UQuickAssetAction::RemoveUnusedAssets() const
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();	// 获取资源
	TArray<FAssetData> UnusedAssetsData;

	FixUpRedirectors();	// 修复重定向器

	for (const FAssetData& SelectedAsset : SelectedAssetsData)
	{
		// 这里获取资源的引用者，需要传递资产路径，在源码里提到：FName 资产路径（AssetPaths）已弃用。使用 GetSoftObjectPath 获取此资产在加载时在内存中使用的路径，或使用 GetObjectPathString（） 来获取 ObjectPathString（）
		TArray<FString> AssetReferences =  UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAsset.GetObjectPathString(), false);	// 获取资源引用者

		// 如果资源没有引用者，则添加到未使用资源列表
		if (AssetReferences.Num() == 0)
		{
			UnusedAssetsData.Add(SelectedAsset);
		}
	}

	if (UnusedAssetsData.Num() == 0)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("No unused assets found"), FText::FromString("Warning"), EAppMsgType::Ok);
		return;
	}

	// 虽然我们这里可以调用UEditorAssetLibrary::DeleteAssets，但是因为这里资产删除是永久性的，所以我希望提供一个用户确认的对话框
	// 所以我们使用ObjectTools::DeleteAssets，这个函数会弹出一个对话框，让用户确认是否删除资源（使用该函数需要引入UnrealEd模块）

	const int32 NumOfAssetDelete = ObjectTools::DeleteAssets(UnusedAssetsData, true);	// 删除未使用的资源

	if (NumOfAssetDelete > 0)
	{
		SM_Debug::ShowNotifyInfo(FText::FromString(FString::Printf(TEXT("Deleted %d unused assets"), NumOfAssetDelete)), FText::FromString("Success"));
	}
	else
	{
		SM_Debug::ShowNotifyInfo(FText::FromString("No unused assets deleted"), FText::FromString("Warning"));
	}

}

void UQuickAssetAction::FixUpRedirectors()
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
