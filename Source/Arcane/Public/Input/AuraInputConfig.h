// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AuraInputConfig.generated.h"

// 这个结构体用来存储输入动作的信息
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;	// 用来绑定输入的输入动作

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();	// 输入动作的标签

};

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* GetInputActionByTag(const FGameplayTag& Tag, bool bLogNotGet = false) const;	// 通过标签获取输入动作

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;	// 用来存储所有的输入动作信息，包含特定输入动作和对应的标签


};
