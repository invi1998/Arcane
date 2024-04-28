// Copyright INVI1998


#include "AbilitySystem/Data/AttributeInfo.h"
#include "Arcane/ArcaneLogChannels.h"

FAuraAttributeInfo UAttributeInfo::GetAttributeInfo(const FGameplayTag AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		// 这里判断Tag是否相等，如果不想用等于，可以使用AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag)来判段也是可以的
		if (AttributeInfo.AttributeTag == AttributeTag)		// 全匹配
		{
			return AttributeInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogArcane, Error, TEXT("AttributeInfo for %s not found in %s"), *AttributeTag.ToString(), *GetName());
	}

	return FAuraAttributeInfo();
}
