// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MenuSaveGame.generated.h"

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
};
