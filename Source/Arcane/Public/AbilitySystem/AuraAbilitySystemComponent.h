// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

// ����һ��ί�У�������Ч��Ӧ�õ�Ŀ��ʱ���ã���ί�����ڹ㲥Ч���ı�ǩ
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// �������������ʱ������֪�������ɫ��AbilityActorInfo�Ѿ���������
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;	// ����һ��ί�У�������Ч��Ӧ�õ�Ŀ��ʱ���ã���ί�����ڹ㲥Ч���ı�ǩ

protected:
	// ��Ч��Ӧ�õ�Ŀ��ʱ����
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
