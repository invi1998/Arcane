// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;

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

	void SaveSlotData(FString SaveSlotName, FString UserName);

	UPROPERTY(EditDefaultsOnly, Category="Save Game")
	TSubclassOf<USaveGame> MenuSaveGameClass;
	
};
