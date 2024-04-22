// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

// 角色职业枚举
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,		// 元素师
	Warrior,			// 战士
	Ranger,				// 游侠
	Zombie,				// 丧尸
};

// 默认角色职业信息
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;	// 主属性效果

	UPROPERTY(EditAnywhere, Category="Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;	// 初始能力
};

/**
 * 角色职业信息数据资产
 */
UCLASS()
class ARCANE_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;	// 角色职业属性信息

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;	// 次要属性效果, 用于所有职业

	UPROPERTY(EditDefaultsOnly, Category= "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;		// 重要属性效果, 用于所有职业

	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass) const;	// 获取角色职业默认信息

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;	// 能力

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCurveTable;	// 伤害计算曲线表

};
