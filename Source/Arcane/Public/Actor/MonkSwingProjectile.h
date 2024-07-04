// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "MonkSwingProjectile.generated.h"

/**
 * 武僧挥砍刀锋
 */
UCLASS()
class ARCANE_API AMonkSwingProjectile : public AAuraProjectile
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnHit() override;
	
};
