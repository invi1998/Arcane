// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "ArcaneEnemySpawnVolume.generated.h"

class UBoxComponent;
class AArcaneEnemySpawnPoint;

UCLASS()
class ARCANE_API AArcaneEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AArcaneEnemySpawnVolume();

	/*
	 * SaveInterface begin
	 * 因为我们的敌人生成体积是一个保存接口，它能够保存当前的敌人生成点是否已经激活过了（即已经生成过敌人了）
	 */
	virtual void LoadActor_Implementation() override;
	/*
	 * SaveInterface end
	 */

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Save Data")
	bool bReached = false;	// 是否已经到达

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	// 盒子重叠

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;	// 盒子组件

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<AArcaneEnemySpawnPoint*> SpawnPoints;	// 生成点

};
