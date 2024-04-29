// Copyright INVI1998


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::GetLevelUpRequiredExp(int32 Level) const
{
	if (LevelUpInformation.IsValidIndex(Level-1))
	{
		return LevelUpInformation[Level - 1].LevelUpRequiredExp;
	}
	return 0;
}

int32 ULevelUpInfo::GetCurrentLevelTotalExp(int32 Level) const
{
	int32 totalExp = 0;
	if (Level <= 0 || LevelUpInformation.Num() <= 1)
	{
		return totalExp;
	}
	for (int32 i = 0; i < Level; i++)
	{
		totalExp += LevelUpInformation[i].LevelUpRequiredExp;
	}
	return totalExp;
}

int32 ULevelUpInfo::GetLevelByExp(int32 Exp) const
{
	int32 level = 1;
	if (Exp <= 0 || LevelUpInformation.Num() <= 1)
	{
		return level;
	}
	for (int32 i = 1; i <= LevelUpInformation.Num(); i++)
	{
		if (Exp >= LevelUpInformation[i].LevelUpRequiredExp)
		{
			Exp -= LevelUpInformation[i].LevelUpRequiredExp;
			level++;
		}
		else
		{
			break;
		}
	}
	return level;
}
