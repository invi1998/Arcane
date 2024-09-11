// Copyright INVI_1998

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickMaterialCreationWidget.generated.h"

// 是否使用通道打包（ORM）
UENUM(BlueprintType)
enum class E_ChannelPackingType : uint8
{
	ECPT_NoChannelPacking		UMETA(DisplayName = "No Channel Packing"),
	ECPT_ORM					UMETA(DisplayName = "OcclusionRoughnessMetallic"),
	ECPT_MAX					UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickMaterialCreationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
#pragma region QuickMaterialCreationCore
	UFUNCTION(BlueprintCallable, Category = "QuickMaterialCreationCore")
	void CreateMaterialFromSelectedTextures();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	E_ChannelPackingType ChannelPackingType = E_ChannelPackingType::ECPT_NoChannelPacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	bool bUserSetMaterialName;		// 如果为true，则使用用户设置的材质名称，否则使用默认的材质名称

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures", meta = (EditCondition = "bUserSetMaterialName"))
	FString MaterialName;	// 用户设置的材质名称

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CreateMaterialFromSelectedTextures")
	bool bCreateMaterialInstance = false;	// 是否创建材质实例

#pragma endregion

#pragma region SupportedTextureFormats
	// 基础颜色
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> BaseColorArray = {
		"_BaseColor",
		"_Albedo",
		"_Diffuse",
		"_Color",
		"_Diff"
	};

	// 金属度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> MetallicArray = {
		"_Metallic",
		"_Metal",
		"_MetallicMap",
		"_MetalMap"
	};

	// 高光
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> SpecularArray = {
		"_Specular",
		"_Spec",
		"_SpecularMap",
		"_SpecMap"
	};

	// 粗糙度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> RoughnessArray = {
		"_Roughness",
		"_Rough",
		"_RoughnessMap",
		"_RoughMap"
	};

	// 各项异性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> AnisotropyArray = {
		"_Anisotropy",
		"_Aniso",
		"_AnisotropyMap",
		"_AnisoMap"
	};

	// 自发光
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> EmissiveArray = {
		"_Emissive",
		"_Emission",
		"_EmissiveMap",
		"_EmissionMap"
	};

	// 法线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> NormalArray = {
		"_Normal",
		"_Normals",
		"_Nrm",
		"_N",
		"_NrmMap",
		"_NormalMap"
	};

	// 切线
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> TangentArray = {
		"_Tangent",
		"_TangentMap",
		"_TangentSpaceNormal"
	};

	// 环境光遮蔽
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> AmbientOcclusionArray = {
		"_AO",
		"_AmbientOcclusion",
		"_Occlusion",
		"_AOMap",
		"_AmbientOcclusionMap"
	};

	// 通道(ORM，纹理的RGB分别存储Occlusion、Roughness、Metallic)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SupportedTextureFormats")
	TArray<FString> ORMArray = {
		"_ORM",
		"_ORMMap",
		"_ORMMap",
		"_ORMMap",
		"_arm",
		"_OcclusionRoughnessMetallic"
	};

#pragma endregion


private:
#pragma region QuickMaterialCreation

	// 处理选中的数据
	bool ProcessSelectedData(const TArray<FAssetData>& SelectedAssetsData, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePath);

	// 检查名称是否有效
	bool CheckIfNameIsValid(const FString& Name, const FString& FolderPath) const;

	// 创建材质
	UMaterial* CreateMaterial(const TArray<UTexture2D*>& TexturesArray, const FString& FolderPath) const;

	
	void Default_CreateMaterialNode(UMaterial* Material, UTexture2D* Texture, uint32& ConnectedPinsNum, float& OffsetX) const;

	void ORM_CreateMaterialNode(UMaterial* Material, UTexture2D* Texture, uint32& ConnectedPinsNum, float& OffsetX) const;

	class UMaterialInstanceConstant* CreateMaterialInstance(const UMaterial* ParentMaterial, const FString& FolderPath) const;

#pragma endregion

#pragma region CreateMaterialNode

	// 连接基础颜色
	bool TryConnectBaseColor(class UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const;

	// 连接金属度
	bool TryConnectMetallic(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const;

	// 连接高光
	bool TryConnectSpecular(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const;

	// 连接粗糙度
	bool TryConnectRoughness(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const;

	// 连接各向异性
	bool TryConnectAnisotropy(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const;

	// 连接法线
	bool TryConnectNormal(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const;

	// 连接环境光遮蔽（AO)
	bool TryConnectAmbientOcclusion(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const;

	bool TryConnectORM(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const;

#pragma endregion

};
