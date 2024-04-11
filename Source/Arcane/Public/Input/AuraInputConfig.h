// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

// ����ṹ�������洢���붯������Ϣ
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;	// ��������������붯��

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();	// ���붯���ı�ǩ

};

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* GetInputActionByTag(const FGameplayTag& Tag, bool bLogNotGet = false) const;	// ͨ����ǩ��ȡ���붯��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;	// �����洢���е����붯����Ϣ�������ض����붯���Ͷ�Ӧ�ı�ǩ


};
