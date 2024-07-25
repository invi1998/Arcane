// Copyright INVI1998


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Game/ArcaneBlueprintFunctionLibrary.h"
#include "Game/ArcaneGameInstance.h"
#include "Game/MenuSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

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

		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();

		// 保存所有实现了 USaveInterface 的 Actor，遍历当前世界中的所有 Actor
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor && Actor->Implements<USaveInterface>())
			{
				FSavedActor NewSavedActor;
				NewSavedActor.ActorName = FName(Actor->GetName());
				NewSavedActor.Transform = Actor->GetActorTransform();

				FMemoryWriter MemoryWriter(NewSavedActor.Bytes);		// 创建一个内存写入器
				FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);	// 创建一个序列化器
				Ar.ArIsSaveGame = true;		// 设置为保存游戏

				// 序列化 Actor
				Actor->Serialize(Ar);		// 将 Actor 序列化到序列化器中（序列化器会将序列化的数据写入到内存写入器中，最终写入到 NewSavedActor.Bytes 中）

				SavedMap.SavedActors.AddUnique(NewSavedActor);
			}
		}

		// 更新保存的地图数据
		for (FSavedMap& Map : SaveGame->SavedMaps)
		{
			if (Map.MapAssertName == WorldName)
			{
				Map = SavedMap;
				break;
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveGame, GameInstance->CurrentSlotName, 0);

	}

}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LevelMaps.Add(DefaultLevelName, DefaultLevelMap);
}
