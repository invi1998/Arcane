// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class USaveGame;

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo>	CharacterClassInfo;		// 角色职业信息

	UPROPERTY(EditDefaultsOnly, Category="Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;				// 技能信息

	UFUNCTION(BlueprintCallable)
	void SaveSlotData(USaveGame* SaveGameObject, FName SlotName, int32 SlotIndex);

	UPROPERTY(EditDefaultsOnly, Category="Save Game")
	TSubclassOf<USaveGame> MenuSaveGameClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString DefaultLevelName;

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultLevelMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> LevelMaps;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> DefaultSaveGameScreen;

	UFUNCTION(BlueprintCallable)
	void TravelToLevel(FString LevelName);

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void BeginPlay() override;
	
};
