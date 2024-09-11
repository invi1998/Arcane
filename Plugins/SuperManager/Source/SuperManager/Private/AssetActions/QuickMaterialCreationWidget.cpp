// Copyright INVI_1998


#include "AssetActions/QuickMaterialCreationWidget.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilityLibrary.h"
#include "AssetToolsModule.h"
#include "Factories/MaterialFactoryNew.h"
#include "Factories/MaterialInstanceConstantFactoryNew.h"
#include "Materials/MaterialExpressionTextureSample.h"
#include "Materials/MaterialInstanceConstant.h"

#pragma region QuickMaterialCreationCore

void UQuickMaterialCreationWidget::CreateMaterialFromSelectedTextures()
{
	if (bUserSetMaterialName)
	{
		if (MaterialName.IsEmpty())
		{
			SM_Debug::ShowMessageDialog(FText::FromString("Material Name is Empty!"), FText::FromString("Error"), EAppMsgType::Ok);
			return;
		}
	}
	else
	{
		MaterialName = "M_NewMaterial";
	}

	const TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<UTexture2D*> SelectedTexturesArray;
	FString SelectedTextureFolderPath;

	if (!ProcessSelectedData(SelectedAssetsData, SelectedTexturesArray, SelectedTextureFolderPath))
	{
		SelectedTexturesArray.Empty();
		SelectedTextureFolderPath.Empty();
		return;
	}

	if (!CheckIfNameIsValid(MaterialName, SelectedTextureFolderPath))
	{
		SelectedTexturesArray.Empty();
		SelectedTextureFolderPath.Empty();
		return;
	}

	if (UMaterial* NewMaterial = CreateMaterial(SelectedTexturesArray, SelectedTextureFolderPath))
	{
		uint32 ConnectedPinsNum = 0;
		float XOffset = 300;
		// 遍历纹理，给材质设置参数
		for (UTexture2D* Texture : SelectedTexturesArray)
		{
			if (!Texture)
			{
				continue;
			}

			switch (ChannelPackingType)
			{
			case E_ChannelPackingType::ECPT_NoChannelPacking:
				{
					Default_CreateMaterialNode(NewMaterial, Texture, ConnectedPinsNum, XOffset);
					break;
				};
			case E_ChannelPackingType::ECPT_ORM:
				{
					ORM_CreateMaterialNode(NewMaterial, Texture, ConnectedPinsNum, XOffset);
					break;
				};
			case E_ChannelPackingType::ECPT_MAX: break;
			default: break;
			}
		}

		if (ConnectedPinsNum > 0 && bCreateMaterialInstance)
		{
			if (UMaterialInstanceConstant* NewMaterialInstance = CreateMaterialInstance(NewMaterial, SelectedTextureFolderPath))
			{
				NewMaterialInstance->SetParentEditorOnly(NewMaterial);		// 设置父材质
			}
		}

	}

	MaterialName.Empty();
}



#pragma endregion

#pragma region QuickMaterialCreation

bool UQuickMaterialCreationWidget::ProcessSelectedData(const TArray<FAssetData>& SelectedAssetsData, TArray<UTexture2D*>& OutSelectedTexturesArray, FString& OutSelectedTexturePath)
{
	if (SelectedAssetsData.Num() == 0)
	{
		SM_Debug::ShowMessageDialog(FText::FromString("No Texture Selected!"), FText::FromString("Error"), EAppMsgType::Ok);
		return false;
	}

	bool bSetMaterialName = false;
	for (const FAssetData& AssetData : SelectedAssetsData)
	{
		if (AssetData.AssetClassPath == UTexture2D::StaticClass()->GetClassPathName())
		{
			if (UTexture2D* Texture = Cast<UTexture2D>(AssetData.GetAsset()))
			{
				OutSelectedTexturesArray.Add(Texture);

				if (OutSelectedTexturePath.IsEmpty())
				{
					OutSelectedTexturePath = AssetData.PackagePath.ToString();
				}

				if (!bUserSetMaterialName && !bSetMaterialName)
				{
					MaterialName = Texture->GetName();
					MaterialName.RemoveFromStart("T_");
					MaterialName.RemoveFromStart("t_");

					MaterialName = "M_" + MaterialName;

					bSetMaterialName = true;
				}
				
			}
			else
			{
				SM_Debug::ShowMessageDialog(FText::FromString( AssetData.AssetName.ToString() + " is not a valid Texture!"), FText::FromString("Error"), EAppMsgType::Ok);
				return false;
			}
		}
		else
		{
			SM_Debug::ShowMessageDialog(FText::FromString("Selected Asset: " + AssetData.AssetName.ToString() + " is not a Texture!"), FText::FromString("Error"), EAppMsgType::Ok);
			return false;
		
		}
	}

	return true;

}

bool UQuickMaterialCreationWidget::CheckIfNameIsValid(const FString& Name, const FString& FolderPath) const
{
	// 检查当前目录下是否有同名的材质
	TArray<FString> ExistingAssetsPaths = UEditorAssetLibrary::ListAssets(FolderPath, false);

	for (const FString& AssetPath : ExistingAssetsPaths)
	{
		const FString ExistingAssetName = FPaths::GetBaseFilename(AssetPath);

		if (ExistingAssetName == Name)
		{
			SM_Debug::ShowMessageDialog(FText::FromString("Material Name: \" " + Name + " \" is already exist!"), FText::FromString("Error"), EAppMsgType::Ok);
			return false;
		}
	}

	return true;
}

UMaterial* UQuickMaterialCreationWidget::CreateMaterial( const TArray<UTexture2D*>& TexturesArray, const FString& FolderPath) const
{
	UMaterial* NewMaterial = nullptr;

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	UMaterialFactoryNew* MaterialFactory = NewObject<UMaterialFactoryNew>();

	UObject* NewObject = AssetToolsModule.Get().CreateAsset(MaterialName, FolderPath, UMaterial::StaticClass(), MaterialFactory);

	if (NewObject)
	{
		NewMaterial = Cast<UMaterial>(NewObject);

		if (NewMaterial)
		{
			// 设置材质参数
			NewMaterial->SetTextureParameterValueEditorOnly(FName("Texture"), TexturesArray[0]);
		}
	}

	return NewMaterial;
}

void UQuickMaterialCreationWidget::Default_CreateMaterialNode(UMaterial* Material, UTexture2D* Texture, uint32& ConnectedPinsNum, float& OffsetX) const
{
	if (UMaterialExpressionTextureSample* TextureSample = NewObject<UMaterialExpressionTextureSample>(Material))
	{
		
		// 如果材质的基础色节点未连接
		if (!Material->GetMaterial()->HasBaseColorConnected())
		{
			// 基础颜色
			if (TryConnectBaseColor(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX+= 300;
				return;
			}
		}

		// 如果材质的金属度节点未连接
		if (!Material->GetMaterial()->HasMetallicConnected())
		{
			// 金属度
			if (TryConnectMetallic(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

		if (!Material->GetMaterial()->HasSpecularConnected())
		{
			// 高光
			if (TryConnectSpecular(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

		if (!Material->GetMaterial()->HasRoughnessConnected())
		{
			// 粗糙度
			if (TryConnectRoughness(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

		if (!Material->GetMaterial()->HasAnisotropyConnected())
		{
			// 各向异性
			if (TryConnectAnisotropy(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

		if (!Material->GetMaterial()->HasNormalConnected())
		{
			// 法线
			if (TryConnectNormal(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

		if (!Material->GetMaterial()->HasAmbientOcclusionConnected())
		{
			// 环境光遮蔽
			if (TryConnectAmbientOcclusion(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

	}
}

void UQuickMaterialCreationWidget::ORM_CreateMaterialNode(UMaterial* Material, UTexture2D* Texture, uint32& ConnectedPinsNum, float& OffsetX) const
{
	if (UMaterialExpressionTextureSample* TextureSample = NewObject<UMaterialExpressionTextureSample>(Material))
	{
		// 如果材质的基础色节点未连接
		if (!Material->GetMaterial()->HasBaseColorConnected())
		{
			// 基础颜色
			if (TryConnectBaseColor(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

		// 如果材质的金属度节点，粗糙度节点，环境光遮蔽节点未连接
		if (!Material->GetMaterial()->HasMetallicConnected() || !Material->GetMaterial()->HasRoughnessConnected() || !Material->GetMaterial()->HasAmbientOcclusionConnected())
		{
			// ORM
			if (TryConnectORM(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

		if (!Material->GetMaterial()->HasNormalConnected())
		{
			// 法线
			if (TryConnectNormal(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}

		if (!Material->GetMaterial()->HasAnisotropyConnected())
		{
			// 各向异性
			if (TryConnectAnisotropy(TextureSample, Texture, Material, OffsetX))
			{
				ConnectedPinsNum++;
				OffsetX += 300;
				return;
			}
		}
	}
}

UMaterialInstanceConstant* UQuickMaterialCreationWidget::CreateMaterialInstance(const UMaterial* ParentMaterial, const FString& FolderPath) const
{
	const FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");

	UMaterialInstanceConstantFactoryNew* MaterialInstanceFactory = NewObject<UMaterialInstanceConstantFactoryNew>();

	FString ParentMaterialName = ParentMaterial->GetName();
	ParentMaterialName.RemoveFromStart("M_");
	ParentMaterialName.InsertAt(0, "MI_");

	UObject* NewObject = AssetToolsModule.Get().CreateAsset(ParentMaterialName + "_Instance", FolderPath, UMaterialInstanceConstant::StaticClass(), MaterialInstanceFactory);

	return Cast<UMaterialInstanceConstant>(NewObject);
}

bool UQuickMaterialCreationWidget::TryConnectBaseColor(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const
{
	for (const FString& BaseColor : BaseColorArray)
	{
		if (Texture->GetName().Contains(BaseColor))
		{
			TextureSample->Texture = Texture;

			Material->GetExpressionCollection().AddExpression(TextureSample);
			Material->GetExpressionInputForProperty(MP_BaseColor)->Connect(0, TextureSample);
			Material->PostEditChange();

			TextureSample->MaterialExpressionEditorX -= OffsetX;

			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectMetallic(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const
{
	for (const FString& Metallic : MetallicArray)
	{
		if (Texture->GetName().Contains(Metallic))
		{
			// 设置纹理参数
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;		// 设置纹理压缩格式（默认，Default (DXT1/5, BC1/3 on DX11)不压缩）
			Texture->SRGB = false;														// 设置是否使用sRGB
			Texture->PostEditChange();

			// 指定纹理
			TextureSample->Texture = Texture;
			TextureSample->SamplerType = SAMPLERTYPE_LinearColor;
			
			// 添加到材质表达式集合
			Material->GetExpressionCollection().AddExpression(TextureSample);
			Material->GetExpressionInputForProperty(MP_Metallic)->Connect(0, TextureSample);
			Material->PostEditChange();

			TextureSample->MaterialExpressionEditorX -= OffsetX;
			TextureSample->MaterialExpressionEditorY += 30;

			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectSpecular(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const
{
	for (const FString& Specular : SpecularArray)
	{
		if (Texture->GetName().Contains(Specular))
		{
			// 设置纹理参数
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;		// 设置纹理压缩格式（默认，Default (DXT1/5, BC1/3 on DX11)不压缩）
			Texture->SRGB = false;														// 设置是否使用sRGB
			Texture->PostEditChange();

			// 指定纹理
			TextureSample->Texture = Texture;
			TextureSample->SamplerType = SAMPLERTYPE_LinearColor;

			// 添加到材质表达式集合
			Material->GetExpressionCollection().AddExpression(TextureSample);
			Material->GetExpressionInputForProperty(MP_Specular)->Connect(0, TextureSample);
			Material->PostEditChange();

			TextureSample->MaterialExpressionEditorX -= OffsetX;
			TextureSample->MaterialExpressionEditorY += 30 * 2;

			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectRoughness(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const
{
	for (const FString& Roughness : RoughnessArray)
	{
		if (Texture->GetName().Contains(Roughness))
		{
			// 设置纹理参数
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;		// 设置纹理压缩格式（默认，Default (DXT1/5, BC1/3 on DX11)不压缩）
			Texture->SRGB = false;														// 设置是否使用sRGB
			Texture->PostEditChange();

			// 指定纹理
			TextureSample->Texture = Texture;
			TextureSample->SamplerType = SAMPLERTYPE_LinearColor;

			// 添加到材质表达式集合
			Material->GetExpressionCollection().AddExpression(TextureSample);
			Material->GetExpressionInputForProperty(MP_Roughness)->Connect(0, TextureSample);
			Material->PostEditChange();

			TextureSample->MaterialExpressionEditorX -= OffsetX;
			TextureSample->MaterialExpressionEditorY += 30 * 3;

			return true;
		}
	}

	return false;
}

bool UQuickMaterialCreationWidget::TryConnectAnisotropy(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const
{
	for (const FString& Anisotropy : AnisotropyArray)
	{
		if (Texture->GetName().Contains(Anisotropy))
		{
			// 设置纹理参数
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;		// 设置纹理压缩格式（默认，Default (DXT1/5, BC1/3 on DX11)不压缩）
			Texture->SRGB = false;														// 设置是否使用sRGB
			Texture->PostEditChange();

			// 指定纹理
			TextureSample->Texture = Texture;
			TextureSample->SamplerType = SAMPLERTYPE_LinearColor;

			// 添加到材质表达式集合
			Material->GetExpressionCollection().AddExpression(TextureSample);
			Material->GetExpressionInputForProperty(MP_Anisotropy)->Connect(0, TextureSample);
			Material->PostEditChange();

			TextureSample->MaterialExpressionEditorX -= OffsetX;
			TextureSample->MaterialExpressionEditorY += 30 * 4;

			return true;
		}
	}
	return false;
}

bool UQuickMaterialCreationWidget::TryConnectNormal(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const
{
	for (const FString& Normal : NormalArray)
	{
		if (Texture->GetName().Contains(Normal))
		{
			// 设置纹理参数
			Texture->CompressionSettings = TextureCompressionSettings::TC_Normalmap;		// 设置纹理压缩格式（法线贴图，Normalmap）
			Texture->SRGB = false;														// 设置是否使用sRGB
			Texture->PostEditChange();

			// 指定纹理
			TextureSample->Texture = Texture;
			TextureSample->SamplerType = SAMPLERTYPE_Normal;

			// 添加到材质表达式集合
			Material->GetExpressionCollection().AddExpression(TextureSample);
			Material->GetExpressionInputForProperty(MP_Normal)->Connect(0, TextureSample);
			Material->PostEditChange();

			TextureSample->MaterialExpressionEditorX -= OffsetX;
			TextureSample->MaterialExpressionEditorY += 30 * 8;

			return true;
		}
	}
	return false;
}

bool UQuickMaterialCreationWidget::TryConnectAmbientOcclusion(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const
{
	for (const FString& AmbientOcclusion : AmbientOcclusionArray)
	{
		if (Texture->GetName().Contains(AmbientOcclusion))
		{
			// 设置纹理参数
			Texture->CompressionSettings = TextureCompressionSettings::TC_Default;		// 设置纹理压缩格式（默认，Default (DXT1/5, BC1/3 on DX11)不压缩）
			Texture->SRGB = false;														// 设置是否使用sRGB
			Texture->PostEditChange();

			// 指定纹理
			TextureSample->Texture = Texture;
			TextureSample->SamplerType = SAMPLERTYPE_LinearColor;

			// 添加到材质表达式集合
			Material->GetExpressionCollection().AddExpression(TextureSample);
			Material->GetExpressionInputForProperty(MP_AmbientOcclusion)->Connect(0, TextureSample);
			Material->PostEditChange();

			TextureSample->MaterialExpressionEditorX -= OffsetX;
			TextureSample->MaterialExpressionEditorY += 30 * 14;

			return true;
		}
	}
	return false;
}

bool UQuickMaterialCreationWidget::TryConnectORM(UMaterialExpressionTextureSample* TextureSample, UTexture2D* Texture, UMaterial* Material, float OffsetX) const
{
	for (const FString& ORM : ORMArray)
	{
		if (Texture->GetName().Contains(ORM))
		{
			// 设置纹理参数
			Texture->CompressionSettings = TextureCompressionSettings::TC_Masks;		// 设置纹理压缩格式（掩码，Masks）
			Texture->SRGB = false;														// 设置是否使用sRGB
			Texture->PostEditChange();

			// 指定纹理
			TextureSample->Texture = Texture;
			TextureSample->SamplerType = SAMPLERTYPE_Masks;			// 设置采样类型(掩码)

			// 添加到材质表达式集合
			Material->GetExpressionCollection().AddExpression(TextureSample);
			Material->GetExpressionInputForProperty(MP_AmbientOcclusion)->Connect(1, TextureSample);		// AO
			Material->GetExpressionInputForProperty(MP_Roughness)->Connect(2, TextureSample);			// Roughness
			Material->GetExpressionInputForProperty(MP_Metallic)->Connect(3, TextureSample);				// Metallic
			Material->PostEditChange();

			TextureSample->MaterialExpressionEditorX -= OffsetX;
			TextureSample->MaterialExpressionEditorY += 30;

			return true;
		}
	}
	return false;
}

#pragma endregion

