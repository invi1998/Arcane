// Copyright INVI1998


#include "Game/AuraGameModeBase.h"

#include "Game/ArcaneGameInstance.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AAuraGameModeBase::SaveSlotData(FString SaveSlotName, FString UserName)
{
}

void AAuraGameModeBase::TravelToLevel(FString LevelName)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, LevelMaps.FindChecked(LevelName));
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	const UArcaneGameInstance* GameInstance = Cast<UArcaneGameInstance>(GetGameInstance());

	if (GameInstance && PlayerStarts.Num() > 0)
	{
		AActor* PlayerStart = PlayerStarts[0];
		for (AActor* Start : PlayerStarts)
		{
			if (APlayerStart* PS = Cast<APlayerStart>(Start))
			{
				if (PS->PlayerStartTag == GameInstance->PlayerStartTag)
				{
					PlayerStart = PS;
					break;
				}
			}
		}
		return PlayerStart;
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LevelMaps.Add(DefaultLevelName, DefaultLevelMap);
}
