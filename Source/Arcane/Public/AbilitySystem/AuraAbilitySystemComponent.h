// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;

// 定义一个委托，用于在效果应用到目标时调用，该委托用于广播效果的标签
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UAuraAbilitySystemComponent*);	// 定义一个委托，用于在给角色添加能力时调用
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);	// 为每个能力定义一个委托

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// 当调用这个函数时，我们知道这个角色的AbilityActorInfo已经被设置了
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;	// 定义一个委托，用于在效果应用到目标时调用，该委托用于广播效果的标签
	FAbilitiesGiven AbilitiesGivenDelegate;	// 定义一个委托，用于在给角色添加能力时调用

	void AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);		// 添加角色的能力，这些能力在角色创建时就会被添加

	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);	// 添加角色的被动能力，这些能力在角色创建时就会被添加

	bool bStartupAbilitiesGiven = false;	// 是否已经添加了角色的能力

	void AbilityInputTagPressed(const FGameplayTag& InputTag);	// 技能输入标签按下

	void AbilityInputTagReleased(const FGameplayTag& InputTag);	// 技能输入标签释放

	void AbilityInputTagHeld(const FGameplayTag& InputTag);	// 技能输入标签按住

	void ForEachAbility(const FForEachAbility& Delegate);	// 为每个能力调用一个委托)

	static FGameplayTag GetAbilityTagBySpec(const FGameplayAbilitySpec& Spec);	// 通过AbilitySpec获取能力标签
	static FGameplayTag GetAbilityInputTagBySpec(const FGameplayAbilitySpec& Spec);	// 通过AbilitySpec获取能力输入标签

	void UpgradeAttribute(FGameplayTag AttributeTag);	// 升级属性

protected:
	// 在效果应用到目标时调用
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
	virtual void OnRep_ActivateAbilities() override;	// 当激活能力时调用，用于在客户端和服务器之间同步激活的能力
};
