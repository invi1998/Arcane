// Copyright INVI1998


#include "Actor/MonkSwingProjectile.h"

void AMonkSwingProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AMonkSwingProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AMonkSwingProjectile::OnHit()
{
	Super::OnHit();
}
