// Copyright INVI1998


#include "AuraGameplayTags.h"

#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;	// ��̬������ʼ��

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	// ���ԭ����ǩ������->��Ҫ����->���ף�����Ϊ�����ܵ����˺�����߸񵲼���
	// �������ǩ��ӵ�GameplayTagsManager��
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage Taken, improves Block Chance"));
	
}
