// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayEffect;
class UGameplayAbility;

// ��ɫְҵö��
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,		// Ԫ��ʦ
	Warrior,			// սʿ
	Ranger,				// ����
	Zombie,				// ɥʬ
};

// Ĭ�Ͻ�ɫְҵ��Ϣ
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;	// ������Ч��

	UPROPERTY(EditAnywhere, Category="Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;	// ��ʼ����
};

/**
 * ��ɫְҵ��Ϣ�����ʲ�
 */
UCLASS()
class ARCANE_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;	// ��ɫְҵ������Ϣ

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;	// ��Ҫ����Ч��, ��������ְҵ

	UPROPERTY(EditDefaultsOnly, Category= "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;		// ��Ҫ����Ч��, ��������ְҵ

	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass) const;	// ��ȡ��ɫְҵĬ����Ϣ

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;	// ����

	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCurveTable;	// �˺��������߱�

};
