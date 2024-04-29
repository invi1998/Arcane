// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequiredExp;		// 升级所需经验

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePoints = 1;			// 属性点

	UPROPERTY(EditDefaultsOnly)
	int32 SkillPoints = 1;				// 技能点

};

/**
 * 等级升级信息
 */
UCLASS()
class ARCANE_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;	// 等级升级信息

	int32 GetLevelUpRequiredExp(int32 Level) const;		// 获取升级所需经验

	int32 GetCurrentLevelTotalExp(int32 Level) const;	// 获取当前等级总经验

	int32 GetLevelByExp(int32 Exp) const;				// 根据经验获取等级

};
