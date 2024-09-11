// Copyright INVI_1998

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
//
//代码在 5.0 之后为了修复重定向器发生了很大变化。以下是虚幻引擎在 5.4 及以上版本中用于修复重定向器的最新版本：
//首先，您需要这些包含项：
#include "AssetToolsModule.h"
#include "AssetViewUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"

#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"	
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Engine/Texture.h"
#include "Blueprint/UserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraEmitter.h"

#include "QuickAssetAction.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)
	void DuplicateSelectedAssets(int32 NumCopies) const;		// 复制选中的资产

	UFUNCTION(CallInEditor)
	void AddPrefixToSelectedAssets() const;		// 为选中的资产添加前缀

	UFUNCTION(CallInEditor)
	void RemoveUnusedAssets() const;		// 移除未使用的资产

private:
	// 资产前缀映射（该映射用于给UE5中所有的资产类型添加前缀）
    TMap<UClass*, FString> AssetPrefixMap = {
	    {UBlueprint::StaticClass(),TEXT("BP_")},
		{UStaticMesh::StaticClass(),TEXT("SM_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstanceConstant::StaticClass(),TEXT("MI_")},
		{UMaterialFunctionInterface::StaticClass(), TEXT("MF_")},
		{UParticleSystem::StaticClass(), TEXT("PS_")},
		{USoundCue::StaticClass(), TEXT("SC_")},
		{USoundWave::StaticClass(), TEXT("SW_")},
		{UTexture::StaticClass(), TEXT("T_")},
		{UTexture2D::StaticClass(), TEXT("T_")},
		{UUserWidget::StaticClass(), TEXT("WBP_")},
		{USkeletalMeshComponent::StaticClass(), TEXT("SK_")},
		{UNiagaraSystem::StaticClass(), TEXT("NS_")},
		{UNiagaraEmitter::StaticClass(), TEXT("NE_")},
    };

	static void FixUpRedirectors();		// 修复重定向器（例如：当资产被移动或重命名时，重定向器会指向新的资产）
	
};
