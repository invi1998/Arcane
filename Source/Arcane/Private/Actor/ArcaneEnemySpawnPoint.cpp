// Copyright INVI1998


#include "Actor/ArcaneEnemySpawnPoint.h"

#include "Character/AuraEnemy.h"

void AArcaneEnemySpawnPoint::SpawnEnemy()
{
	if (EnemyClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;	// 如果可能则调整，但总是生成

		if (AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform()))
		{
			Enemy->SetCharacterClass(EnemyCharacterClass);
			Enemy->SetCharacterLevel(EnemyLevel);
			Enemy->FinishSpawning(GetActorTransform());
			Enemy->SpawnDefaultController();
		}
	}
}
