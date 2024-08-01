// Copyright INVI1998


#include "Actor/ArcaneEnemySpawnVolume.h"

#include "Actor/ArcaneEnemySpawnPoint.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

AArcaneEnemySpawnVolume::AArcaneEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Box->SetCollisionObjectType(ECC_WorldStatic);		// 世界静态
}

void AArcaneEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		// Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Destroy();
	}
}

void AArcaneEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	Box->OnComponentBeginOverlap.AddDynamic(this, &AArcaneEnemySpawnVolume::OnBoxOverlap);
	
}

void AArcaneEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;

	bReached = true;

	for (AArcaneEnemySpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnPoint)
		{
			SpawnPoint->SpawnEnemy();
		}
	}

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

