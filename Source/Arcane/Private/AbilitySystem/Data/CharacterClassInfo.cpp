// Copyright INVI1998


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(ECharacterClass CharacterClass) const
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
