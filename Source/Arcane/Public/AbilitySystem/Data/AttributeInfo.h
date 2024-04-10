// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

// ���ﴴ��һ���ṹ�壬�����洢�������Ե������Ϣ��һ�����Է����仯�����ǾͿ��Խ�����ṹ�崫�ݸ�UI����UI����
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	// GamelayTag
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	// ������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeName = FText();

	// ����������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeChineseName = FText();

	// ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// ����ֵ
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.0f;

	// ����Icon
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
	// ����һ��GameplayTag������һ��FAuraAttributeInfo��bLogNotFound ���Ϊtrue�����Ҳ�������ʱ���ӡ������Ϣ����¼��־
	FAuraAttributeInfo GetAttributeInfo(const FGameplayTag AttributeTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)	// ֻ��
	TArray<FAuraAttributeInfo> AttributeInformation;	// ������Ϣ


};
