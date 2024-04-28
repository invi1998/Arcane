// Copyright INVI1998


#include "AbilitySystem/Data/AbilityInfo.h"
#include "Arcane/ArcaneLogChannels.h"

FAuraAbility UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbility& Ability : AbilitiesInformation)
	{
		if (Ability.AbilityTag == AbilityTag)
		{
			return Ability;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogArcane, Error, TEXT("Ability with tag %s not found in AbilitiesInformation %s"), *AbilityTag.ToString(), *GetName());
	}

	return FAuraAbility();
}
