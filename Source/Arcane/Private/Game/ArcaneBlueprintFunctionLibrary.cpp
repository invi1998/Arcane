// Copyright INVI1998


#include "Game/ArcaneBlueprintFunctionLibrary.h"

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

void UArcaneBlueprintFunctionLibrary::CreateNewGameSave(const UObject* WorldContextObject, FName PlayerName,
	FString SlotPrefix)
{
	int32 LocCheckedSlot = 0;
	FString SlotName = SlotPrefix + FString::FromInt(LocCheckedSlot);

	while (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		LocCheckedSlot++;
		SlotName = SlotPrefix + FString::FromInt(LocCheckedSlot);
	}

	// 创建新存档
}
