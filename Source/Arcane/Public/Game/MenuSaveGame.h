// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "MenuSaveGame.generated.h"

class UGameplayAbility;

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Map Assert Name"))
	FString MapAssertName;
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


/*
 * player Data (玩家数据)
 */
USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 PlayerLevel = 1;	// 玩家等级

	UPROPERTY()
	int32 PlayerExp = 0;	// 玩家经验

	UPROPERTY()
	int32 SpellPoints = 0;	// 技能点

	UPROPERTY()
	int32 AttributePoints = 0;	// 属性点

	UPROPERTY()
	float Strength = 0;	// 力量

	UPROPERTY()
	float Agility = 0;	// 敏捷

	UPROPERTY()
	float Intelligence = 0;	// 智力

	UPROPERTY()
	float Vigor = 0;	// 体力

	UPROPERTY()
	float Resilience = 0;	// 抗性
	
};

/*
 * player Ability (玩家技能)
 */
USTRUCT(BlueprintType)
struct FPlayerSavedAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;	// 技能

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityTag = FGameplayTag::EmptyTag;	// 技能标签

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityStatus = FGameplayTag::EmptyTag;	// 技能状态标签

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilitySlot = FGameplayTag::EmptyTag;	// 技能槽标签

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityType = FGameplayTag::EmptyTag;	// 技能类型标签

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	int32 AbilityLevel = 1;			// 技能等级

};

// 重载==运算符，用于比较两个FPlayerSavedAbility是否相等，用于TArray::AddUnique
inline bool operator==(const FPlayerSavedAbility& Lhs, const FPlayerSavedAbility& Rhs)
{
	return Lhs.AbilityTag.MatchesTagExact(Rhs.AbilityTag);
}

/*
 * 世界场景存档
 */
USTRUCT(BlueprintType)
struct FSavedActor
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName ActorName = NAME_None;

	UPROPERTY()
	FTransform Transform = FTransform();

	// Serialized variables from the actor - only those marked with SaveGame Specifiers （从Actor序列化的变量 - 仅标记了SaveGame Specifiers的变量）
	UPROPERTY()
	TArray<uint8> Bytes;

};

inline bool operator==(const FSavedActor& Lhs, const FSavedActor& Rhs)
{
	return Lhs.ActorName == Rhs.ActorName;
}

USTRUCT(BlueprintType)
struct FSavedMap
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString MapAssertName = FString();

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

inline bool operator==(const FSavedMap& Lhs, const FSavedMap& Rhs)
{
	return Lhs.MapAssertName == Rhs.MapAssertName;
}



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

	UPROPERTY()
	FPlayerData PlayerData;

	UPROPERTY()
	TArray<FPlayerSavedAbility> SavedPlayerAbilities;

	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	FSavedMap GetSavedMapWithMapName(const FString& InMapName);

	bool HasMap(const FString& InMapName);

};
