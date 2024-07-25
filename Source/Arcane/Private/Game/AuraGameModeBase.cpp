// Copyright INVI1998


#include "Game/AuraGameModeBase.h"

#include "Game/ArcaneBlueprintFunctionLibrary.h"
#include "Game/ArcaneGameInstance.h"
#include "Game/MenuSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void AAuraGameModeBase::SaveSlotData(USaveGame* SaveGameObject, FName SlotName, int32 SlotIndex)
{
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName.ToString(), SlotIndex);
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

UMenuSaveGame* AAuraGameModeBase::GetCurrentSaveGame() const
{
	const UArcaneGameInstance* GameInstance = Cast<UArcaneGameInstance>(GetGameInstance());

	return GameInstance->LoadedGame;
}

void AAuraGameModeBase::SaveInGameProgressData(const UMenuSaveGame* SaveGameObject) const
{
	if (UArcaneGameInstance* GameInstance = Cast<UArcaneGameInstance>(GetGameInstance()))
	{
		GameInstance->PlayerStartTag = SaveGameObject->SavedGameInfo.PlayerStartTag;

		GameInstance->OnSaveCurrentGame();
	}
}

void AAuraGameModeBase::SaveWorldState(UWorld* World)
{
	FString WorldName = World->GetName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);	// 移除 StreamingLevelsPrefix 前缀

	const UArcaneGameInstance* GameInstance = Cast<UArcaneGameInstance>(GetGameInstance());
	check(GameInstance);

	if (UMenuSaveGame* SaveGame = GetCurrentSaveGame())
	{
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssertName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}
	}

}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LevelMaps.Add(DefaultLevelName, DefaultLevelMap);
}
