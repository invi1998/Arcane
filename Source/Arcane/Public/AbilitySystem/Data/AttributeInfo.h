// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

// 这里创建一个结构体，用来存储给定属性的相关信息，一但属性发生变化，我们就可以将这个结构体传递给UI，让UI更新
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	// GamelayTag
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	// 属性名
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeName = FText();

	// 属性中文名
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeChineseName = FText();

	// 属性描述
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// 属性值
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;

	// 属性Icon
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* AttributeIcon = nullptr;
};


/**
 * 
 */
UCLASS()
class ARCANE_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	// 传入一个GameplayTag，返回一个FAuraAttributeInfo，bLogNotFound 如果为true，当找不到属性时会打印错误信息，记录日志
	FAuraAttributeInfo GetAttributeInfo(const FGameplayTag AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	// 只读
	TArray<FAuraAttributeInfo> AttributeInformation;	// 属性信息


};
