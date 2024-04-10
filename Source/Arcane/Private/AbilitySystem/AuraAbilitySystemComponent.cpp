// Copyright INVI1998


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    // 一但主角或者Enemy的AbilityActorInfo被设置，就代表他们的AbilitySystemComponent已经被初始化了，这时候我们就可以绑定EffectApplied委托了

    // 绑定EffectApplied委托，该委托在效果应用到目标时调用
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

    const FAuraGameplayTags& GameplayTag = FAuraGameplayTags::Get();

	// 打印 GameplayTag.Attributes_Secondary_Armor.ToString();
    // GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, GameplayTag.Attributes_Secondary_Armor.ToString());
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTagContainer;    // 创建一个GameplayTagContainer
    // 通过绑定委托，获取到GameplayTag，然后将其添加到AbilitySystemComponent的AssetTags中
    EffectSpec.GetAllAssetTags(AssetTagContainer);

    EffectAssetTags.Broadcast(AssetTagContainer);    // 广播这些Tag到UI

    for (const FGameplayTag& Tag : AssetTagContainer)
    {
	    // TODO: 广播这些Tag到UI
        // UKismetSystemLibrary::PrintString(this, Tag.ToString(), true, true, FLinearColor::Gray, 5.0f);
    }
}
