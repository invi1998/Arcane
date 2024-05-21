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
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;		// 技能标签

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag::EmptyTag;			// 输入标签（用于绑定输入），该值蓝图不可编辑，通过代码逻辑设置

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;		// 冷却标签

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;					// 技能图标

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Background = nullptr;			// 技能背景

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StateTag = FGameplayTag::EmptyTag;			// 技能状态标签

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequired = 1;								// 解锁技能对玩家的等级要求

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AbilityClass = nullptr;	// 技能类
};

/**
 * 技能信息
 */
UCLASS()
class ARCANE_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AbilityInformation")
	TArray<FAuraAbilityInfo> AbilitiesInformation;						// 技能列表


	FAuraAbilityInfo FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound = false) const;
	
};
