// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;		// ���ܱ�ǩ

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;			// �����ǩ�����ڰ����룩����ֵ��ͼ���ɱ༭��ͨ�������߼�����

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;		// ��ȴ��ǩ

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;					// ����ͼ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Background = nullptr;			// ���ܱ���

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StateTag = FGameplayTag::EmptyTag;			// ����״̬��ǩ

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequired = 1;								// �������ܶ���ҵĵȼ�Ҫ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass = nullptr;	// ������
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
	TArray<FAuraAbilityInfo> AbilitiesInformation;						// �����б�


	FAuraAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
};
