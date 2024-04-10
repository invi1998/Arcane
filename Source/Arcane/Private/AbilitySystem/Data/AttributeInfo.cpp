// Copyright INVI1998


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::GetAttributeInfo(const FGameplayTag AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		// �����ж�Tag�Ƿ���ȣ���������õ��ڣ�����ʹ��AttributeInfo.AttributeTag.MatchesTagExact(AttributeTag)���ж�Ҳ�ǿ��Ե�
		if (AttributeInfo.AttributeTag == AttributeTag)		// ȫƥ��
		{
			return AttributeInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeInfo for %s not found in %s"), *AttributeTag.ToString(), *GetName());
	}

	return FAuraAttributeInfo();
}
