// Copyright INVI1998


#include "Checkpoint/Checkpoint.h"

#include "Components/SphereComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"


ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	PrimaryActorTick.bCanEverTick = false;

	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckpointMesh"));
	CheckpointMesh->SetupAttachment(GetRootComponent());
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	CheckpointMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(CheckpointMesh);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);

}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 只有玩家角色进入的时候才会触发
	if (OtherActor->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);		// 保存玩家游戏进度
		HandleGlowEffects();
	}
}

void ACheckpoint::HandleGlowEffects()
{
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 创建一个动态材质实例，基于当前的材质
	UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
	CheckpointMesh->SetMaterial(0, DynamicMaterial);

	CheckpointReached(DynamicMaterial);
}


