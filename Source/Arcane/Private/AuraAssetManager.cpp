// Copyright INVI1998


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
// #include "AbilitySystemGlobals.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	checkf(GEngine, TEXT("UAuraAssetManager::Get() called with GEngine == nullptr.  This function should only be called after Engine has been initialized."));
	UAuraAssetManager* AuraAssetManagerSingleton = Cast<UAuraAssetManager>(GEngine->AssetManager);	// ��GEngine->AssetManagerת��ΪUAuraAssetManager����
	return *AuraAssetManagerSingleton;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();	// ��ʼ����Ϸ��ǩ

	// ����ʹ��AbilitySystem��Target Data������Ŀ��������Ҫ��ʼ��ȫ�����ݣ����Ǻ��б�Ҫ��
	// Tips: ʵ���ϣ��� 5.3 �汾�в���Ҫ��
	// UAbilitySystemGlobals::Get().InitGlobalData();	// ��ʼ��ȫ�����ݣ�

	// ����Ҫ�������һ�������ǽ����AssetManagerע�ᵽGEngine�У�����Ϊ���ǵ���Ŀ���ʲ�������
}
