// Copyright INVI1998


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    // һ�����ǻ���Enemy��AbilityActorInfo�����ã��ʹ������ǵ�AbilitySystemComponent�Ѿ�����ʼ���ˣ���ʱ�����ǾͿ��԰�EffectAppliedί����

    // ��EffectAppliedί�У���ί����Ч��Ӧ�õ�Ŀ��ʱ����
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);

    const FAuraGameplayTags& GameplayTag = FAuraGameplayTags::Get();

	// ��ӡ GameplayTag.Attributes_Secondary_Armor.ToString();
    // GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, GameplayTag.Attributes_Secondary_Armor.ToString());
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTagContainer;    // ����һ��GameplayTagContainer
    // ͨ����ί�У���ȡ��GameplayTag��Ȼ������ӵ�AbilitySystemComponent��AssetTags��
    EffectSpec.GetAllAssetTags(AssetTagContainer);

    EffectAssetTags.Broadcast(AssetTagContainer);    // �㲥��ЩTag��UI

    for (const FGameplayTag& Tag : AssetTagContainer)
    {
	    // TODO: �㲥��ЩTag��UI
        // UKismetSystemLibrary::PrintString(this, Tag.ToString(), true, true, FLinearColor::Gray, 5.0f);
    }
}
