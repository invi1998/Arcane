// Copyright INVI1998


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	checkf(GEngine, TEXT("UAuraAssetManager::Get() called with GEngine == nullptr.  This function should only be called after Engine has been initialized."));
	UAuraAssetManager* AuraAssetManagerSingleton = Cast<UAuraAssetManager>(GEngine->AssetManager);	// 将GEngine->AssetManager转换为UAuraAssetManager类型
	return *AuraAssetManagerSingleton;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();	// 初始化游戏标签

	// 现在要做的最后一步，就是将这个AssetManager注册到GEngine中，设置为我们的项目的资产管理器
}
