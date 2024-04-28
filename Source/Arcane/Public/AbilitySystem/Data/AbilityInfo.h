// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;		// ���ܱ�ǩ

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;			// �����ǩ�����ڰ����룩����ֵ��ͼ���ɱ༭��ͨ�������߼�����

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;					// ����ͼ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Background = nullptr;			// ���ܱ���

};

/**
 * ������Ϣ
 */
UCLASS()
class ARCANE_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FAuraAbility> AbilitiesInformation;						// �����б�


	FAuraAbility FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
};
