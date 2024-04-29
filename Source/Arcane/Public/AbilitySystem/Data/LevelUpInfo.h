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
	int32 LevelUpRequiredExp;		// �������辭��

	UPROPERTY(EditDefaultsOnly)
	int32 AttributePoints = 1;			// ���Ե�

	UPROPERTY(EditDefaultsOnly)
	int32 SkillPoints = 1;				// ���ܵ�

};

/**
 * �ȼ�������Ϣ
 */
UCLASS()
class ARCANE_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;	// �ȼ�������Ϣ

	int32 GetLevelUpRequiredExp(int32 Level) const;		// ��ȡ�������辭��

	int32 GetCurrentLevelTotalExp(int32 Level) const;	// ��ȡ��ǰ�ȼ��ܾ���

	int32 GetLevelByExp(int32 Exp) const;				// ���ݾ����ȡ�ȼ�

};
