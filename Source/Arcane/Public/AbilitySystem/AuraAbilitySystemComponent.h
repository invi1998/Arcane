// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraGameplayAbility;

// 定义一个委托，用于在效果应用到目标时调用，该委托用于广播效果的标签
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /* Asset Tags */);

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

	void AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities);		// 添加角色的能力，这些能力在角色创建时就会被添加

	void AbilityInputTagPressed(const FGameplayTag& InputTag);	// 技能输入标签按下

	void AbilityInputTagReleased(const FGameplayTag& InputTag);	// 技能输入标签释放

	void AbilityInputTagHeld(const FGameplayTag& InputTag);	// 技能输入标签按住

protected:
	// 在效果应用到目标时调用
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
