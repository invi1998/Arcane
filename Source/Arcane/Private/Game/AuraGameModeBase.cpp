// Copyright INVI1998


#include "Game/AuraGameModeBase.h"

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	LevelMaps.Add(DefaultLevelName, DefaultLevelMap);
}
