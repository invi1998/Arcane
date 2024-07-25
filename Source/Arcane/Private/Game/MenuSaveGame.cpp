// Copyright INVI1998


#include "Game/MenuSaveGame.h"

FSavedMap UMenuSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (const FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapAssertName == InMapName)
		{
			return SavedMap;
		}
	}

	return FSavedMap();
}

bool UMenuSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapAssertName == InMapName)
		{
			return true;
		}
	}

	return false;
}
