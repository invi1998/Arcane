// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()

public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapEntrance")
	TSoftObjectPtr<UWorld> DestinationMap;		// 目的地地图

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MapEntrance")
	FName DestinationPlayerStartTag;				// 目的地生成点

	/*
	 * SaveGameInterface Start
	 */
	virtual void LoadActor_Implementation() override;
	/*
	 * SaveGameInterface End
	 */

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
