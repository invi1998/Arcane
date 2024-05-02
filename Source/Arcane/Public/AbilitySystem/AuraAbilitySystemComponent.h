// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;

// ����һ��ί�У�������Ч��Ӧ�õ�Ŀ��ʱ���ã���ί�����ڹ㲥Ч���ı�ǩ
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UAuraAbilitySystemComponent*);	// ����һ��ί�У������ڸ���ɫ�������ʱ����
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);	// Ϊÿ����������һ��ί��

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
	FAbilitiesGiven AbilitiesGivenDelegate;	// ����һ��ί�У������ڸ���ɫ�������ʱ����

	void AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);		// ��ӽ�ɫ����������Щ�����ڽ�ɫ����ʱ�ͻᱻ���

	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);	// ��ӽ�ɫ�ı�����������Щ�����ڽ�ɫ����ʱ�ͻᱻ���

	bool bStartupAbilitiesGiven = false;	// �Ƿ��Ѿ�����˽�ɫ������

	void AbilityInputTagPressed(const FGameplayTag& InputTag);	// ���������ǩ����

	void AbilityInputTagReleased(const FGameplayTag& InputTag);	// ���������ǩ�ͷ�

	void AbilityInputTagHeld(const FGameplayTag& InputTag);	// ���������ǩ��ס

	void ForEachAbility(const FForEachAbility& Delegate);	// Ϊÿ����������һ��ί��)

	static FGameplayTag GetAbilityTagBySpec(const FGameplayAbilitySpec& Spec);	// ͨ��AbilitySpec��ȡ������ǩ
	static FGameplayTag GetAbilityInputTagBySpec(const FGameplayAbilitySpec& Spec);	// ͨ��AbilitySpec��ȡ���������ǩ

	void UpgradeAttribute(FGameplayTag AttributeTag);	// ��������

protected:
	// ��Ч��Ӧ�õ�Ŀ��ʱ����
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	virtual void OnRep_ActivateAbilities() override;	// ����������ʱ���ã������ڿͻ��˺ͷ�����֮��ͬ�����������
};
