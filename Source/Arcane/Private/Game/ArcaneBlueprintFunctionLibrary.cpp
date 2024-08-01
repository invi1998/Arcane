// Copyright INVI1998


#include "Game/ArcaneBlueprintFunctionLibrary.h"

#include "Game/AuraGameModeBase.h"
#include "Game/MenuSaveGame.h"
#include "Kismet/GameplayStatics.h"

TArray<FString> UArcaneBlueprintFunctionLibrary::FindAllSaveGames(const UObject* WorldContextObject, FString SlotPrefix)
{
	TArray<FString> SaveGameNames;
	for (int32 i = 0; i < 100; i++)
	{
		FString SlotName = SlotPrefix + FString::FromInt(i);
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			SaveGameNames.Add(SlotName);
		}
	}

	// 同时，将存档按照存档时间排序 (遍历所有存档，获取存档时间，然后排序)
	for (int32 i = 0; i < SaveGameNames.Num(); i++)
	{
		for (int32 j = i + 1; j < SaveGameNames.Num(); j++)
		{
			const UMenuSaveGame* SaveGameA = Cast<UMenuSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameNames[i], 0));
			const UMenuSaveGame* SaveGameB = Cast<UMenuSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameNames[j], 0));
			if (SaveGameA->SaveGameSlot.DateTime < SaveGameB->SaveGameSlot.DateTime)
			{
				FString Temp = SaveGameNames[i];
				SaveGameNames[i] = SaveGameNames[j];
				SaveGameNames[j] = Temp;
			}
		}
	}

	return SaveGameNames;
}

UMenuSaveGame* UArcaneBlueprintFunctionLibrary::CreateNewGameSave(const UObject* WorldContextObject, FName PlayerName, TSubclassOf<USaveGame> SaveGameClass, FString SlotPrefix)
{
	int32 LocCheckedSlot = 0;
	FString SlotName = SlotPrefix + FString::FromInt(LocCheckedSlot);

	while (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		LocCheckedSlot++;
		SlotName = SlotPrefix + FString::FromInt(LocCheckedSlot);
	}

	// 创建新存档
	UMenuSaveGame* NewSaveGame = Cast<UMenuSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));

	if (AuraGameMode && NewSaveGame)
	{
		// save game slot
		NewSaveGame->SaveGameSlot.DateTime = FDateTime::Now();
		NewSaveGame->SaveGameSlot.PlayerName = PlayerName;
		NewSaveGame->SaveGameSlot.SlotName = FName(SlotName);
		NewSaveGame->SaveGameSlot.Level = 1;
		NewSaveGame->SaveGameSlot.Map = AuraGameMode->DefaultLevelName;
		NewSaveGame->SaveGameSlot.QuestImage = AuraGameMode->DefaultSaveGameScreen;
		NewSaveGame->SaveGameSlot.MapAssertName = AuraGameMode->DefaultLevelMap.ToSoftObjectPath().GetAssetName();

		UGameplayStatics::SaveGameToSlot(NewSaveGame, SlotName, 0);
	}

	return NewSaveGame;
}

UMenuSaveGame* UArcaneBlueprintFunctionLibrary::LoadSpecificSlot(const UObject* WorldContextObject, FString SlotName)
{
	UMenuSaveGame* SaveGame = Cast<UMenuSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	return SaveGame;
}


