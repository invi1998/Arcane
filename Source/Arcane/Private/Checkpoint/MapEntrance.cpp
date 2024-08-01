// Copyright INVI1998


#include "Checkpoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"


AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CollisionSphere->SetupAttachment(MoveToComponent);
}

void AMapEntrance::LoadActor_Implementation()
{
	// 不做任何事情
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 只有玩家角色进入的时候才会触发
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (const AAuraGameModeBase* GameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			// 保存我们将去的地图

			GameMode->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}

		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);		// 保存玩家游戏进度

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);

		if (const AAuraPlayerController* PlayerController = Cast<AAuraPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
			{
				AuraHUD->ShowOverlayWidget();
			}
		}

	}
}
