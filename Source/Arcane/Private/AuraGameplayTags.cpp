// Copyright INVI1998


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;	// ��̬������ʼ��

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// ���ԭ����ǩ������->��Ҫ����->���ף�����Ϊ�����ܵ����˺�����߸񵲼���
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage Taken, improves Block Chance"));
	// ���ԭ����ǩ������->��Ҫ����->���ף�����Ϊ�����ܵ����˺�����߸񵲼���
}
