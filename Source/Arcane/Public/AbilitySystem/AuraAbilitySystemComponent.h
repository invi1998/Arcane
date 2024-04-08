// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

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

protected:
	// 在效果应用到目标时调用
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
	
};
