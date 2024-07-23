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


/** SaveSetting 设置保存 */

/** Audio（音频设置) */
USTRUCT(BlueprintType)
struct FStructAudioSave
{
	GENERATED_BODY()
public:
	/** 声音开启 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Sounds Enabled"))
	bool SoundsEnabled;

	/** 主体音量 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Master Volume"))
	float MasterVolume;

	/** BGM开启 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Music Enabled"))
	bool MusicEnabled;

	/** BGM音量 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Music Volume"))
	float MusicVolume;

	/** 音效 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Sound Effects Enabled"))
	bool SoundEffectsEnabled;

	/** 音效音量 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Sound Effects Volume"))
	float SoundEffectsVolume;

	/** UI音效 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "UI Sounds Enabled"))
	bool UISoundsEnabled;

	/** 用户接口音量 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "User Interface Volume"))
	float UserInterfaceVolume;
};

/** 游戏设置 */
USTRUCT(BlueprintType)
struct FStructGameplaySettingsSave
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Show HUD"))
	bool ShowHUD;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Chat Log"))
	FName ChatLog;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Minimap"))
	bool Minimap;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Vehicle Seat Info"))
	bool VehicleSeatInfo;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Awards"))
	bool Awards;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Crosshair Color"))
	FLinearColor CrosshairColor = FLinearColor::White;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Crosshair Opacity"))
	float CrosshairOpacity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Kill Log"))
	bool KillLog;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Kill Log Filter"))
	FName KillLogFilter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Kill Log Weapon"))
	FName KillLogWeapon;
};

/** 鼠标设置 */
USTRUCT(BlueprintType)
struct FStructMouseSave
{
	GENERATED_BODY()
public:
	/* 鼠标翻转 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Invert Mouse"))
	bool InvertMouse;

	/** 鼠标灵敏度 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Mouse Sensitivity"))
	float MouseSensitivity = 1.0f;
};

/** 视频设置 */
USTRUCT(BlueprintType)
struct FStructVideoSave
{
	GENERATED_BODY()
public:
	/** 运动模糊 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Motion Blur"))
	bool MotionBlur;

	/** gama值 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Gamma"))
	float Gamma = 2.2f;
};

/** 游戏设置保存 */
USTRUCT(BlueprintType)
struct FStructGameSave
{
	GENERATED_BODY()
public:
	/** 鼠标设置 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Mouse Settings"))
	FStructMouseSave MouseSettings;

	/** 游戏设置 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Gameplay Settings"))
	FStructGameplaySettingsSave GameplaySettings;
};

/** 设置 */
USTRUCT(BlueprintType)
struct FStructSettingsSave
{
	GENERATED_BODY()
public:
	/** 音频n */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Audio Settings"))
	FStructAudioSave AudioSettings;

	/** 视频 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Video Settings"))
	FStructVideoSave VideoSettings;
};






/**
 * 蓝图可见的存档类
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FStructSettingsSave Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FStructGameSave Gameplay;

};
