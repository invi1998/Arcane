// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "ArcaneEnemySpawnPoint.generated.h"

class AAuraEnemy;

/**
 * 
 */
UCLASS()
class ARCANE_API AArcaneEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AAuraEnemy> EnemyClass;		// 敌人类

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 EnemyLevel;		// 敌人等级

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	ECharacterClass EnemyCharacterClass = ECharacterClass::Goblin;		// 敌人职业

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void SpawnEnemy();
	
};
