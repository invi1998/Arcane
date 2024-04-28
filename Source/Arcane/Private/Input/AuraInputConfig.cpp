// Copyright INVI1998


#include "Input/AuraInputConfig.h"
#include "Arcane/ArcaneLogChannels.h"

const UInputAction* UAuraInputConfig::GetInputActionByTag(const FGameplayTag& Tag, bool bLogNotGet) const
{
	for (const FAuraInputAction& AuraInputAction : AbilityInputActions)
	{
		if (AuraInputAction.InputAction && AuraInputAction.InputTag == Tag)
		{
			return AuraInputAction.InputAction;
		}
	}

	if (bLogNotGet)
	{
		UE_LOG(LogArcane, Warning, TEXT("InputAction with Tag %s not found in %s"), *Tag.ToString(), *GetName());
	}

	return nullptr;
}
