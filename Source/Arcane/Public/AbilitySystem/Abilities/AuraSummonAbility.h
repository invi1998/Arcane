// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * �ٻ��༼��
 */
UCLASS()
class ARCANE_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Summoning")
	int32 NumMinions = 5;	// �ٻ�������

	UPROPERTY(EditAnywhere, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;	// �ٻ�����

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float MinSpawnDistance = 400.f;		// ��С�ٻ�����

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float MaxSpawnDistance = 600.0f;	// ����ٻ�����

	UPROPERTY(EditAnywhere, Category = "Summoning")
	float SpawnSpread = 100.0f;	// �ٻ���ɢ��(�ֲ�), ��ֵԽ��, �ٻ������λ�÷�ΧԽ��

	UFUNCTION(BlueprintCallable, Category = "Summoning")
	TArray<FVector> GetSpawnLocations();	// ��ȡ�ٻ���λ��

	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();	// ��ȡ������ٻ�������
};
