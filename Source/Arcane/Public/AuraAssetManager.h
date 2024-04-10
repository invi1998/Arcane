// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UAuraAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;	// 在这里加载资源，这个函数会在引擎启动时调用，所以这个函数时调用初始化本地Tags最好的地方
	
};
