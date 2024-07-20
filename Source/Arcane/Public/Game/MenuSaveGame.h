// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MenuSaveGame.generated.h"

/** 存档Slot数据 */
USTRUCT(BlueprintType)
struct FSaveSlot
{
	GENERATED_BODY()
public:
	/** 插槽名 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Slot Name", MakeStructureDefaultValue = "None"))
	FName SlotName;

	/** 存档时间 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Date Time", MakeStructureDefaultValue = "0001.01.01-00.00.00"))
	FDateTime DateTime;

	/** 存档名称 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Player Name", MakeStructureDefaultValue = "None"))
	FName PlayerName;

	/** 存档背景 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Quest Image", MakeStructureDefaultValue = "None"))
	TObjectPtr<UTexture2D> QuestImage;

	/** 角色等级 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level", MakeStructureDefaultValue = "0"))
	int32 Level;

	/** 关卡名 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Map"))
	FString Map;
};

/** 存档数据 */
USTRUCT(BlueprintType)
struct FSavedGameInfo
{
	GENERATED_BODY()
public:
	/** 插槽名 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Slot Name", MakeStructureDefaultValue = "None"))
	FName SlotName;

	/** 存档时间 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Date Time", MakeStructureDefaultValue = "0001.01.01-00.00.00"))
	FDateTime DateTime;

	/** 存档名称 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Player Name", MakeStructureDefaultValue = "None"))
	FName PlayerName;

	/** 存档背景 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Quest Image", MakeStructureDefaultValue = "None"))
	TObjectPtr<UTexture2D> QuestImage;

	/** 角色等级 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Level", MakeStructureDefaultValue = "0"))
	int32 Level;

	/** 关卡名 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Map"))
	FString Map;

	/** 是否是新游戏 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Is New Game", MakeStructureDefaultValue = "True"))
	bool IsNewGame;

	/** 角色位置 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (DisplayName = "Player Transform"))
	FTransform PlayerTransform;

	/** 角色起始生成标记 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, SaveGame, meta = (DisplayName = "Player Spawn Point"))
	FName PlayerStartTag;
};



/**
 * 
 */
UCLASS()
class ARCANE_API UMenuSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SaveSlotName = FString();

	UPROPERTY()
	FString UserName = FString();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FSaveSlot SaveGameSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FSavedGameInfo SavedGameInfo;

};
