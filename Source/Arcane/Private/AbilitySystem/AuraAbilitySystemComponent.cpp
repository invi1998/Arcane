// Copyright INVI1998


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
    // һ�����ǻ���Enemy��AbilityActorInfo�����ã��ʹ������ǵ�AbilitySystemComponent�Ѿ�����ʼ���ˣ���ʱ�����ǾͿ��԰�EffectAppliedί����

    // ��EffectAppliedί�У���ί����Ч��Ӧ�õ�Ŀ��ʱ����
    OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);

    // ���ί��ֻ���ڷ���˱����ã�����������ڿͻ���Ҳ�ܵ��ã���ô������Ҫ�����ί������ΪClient��Reliable
    // �ͻ���RPC����Ƴ��ڷ���˵��ã�Ȼ�������ٹ㲥�����пͻ��ˣ��ڿͻ���ִ��

}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UAuraGameplayAbility>>& StartupAbilities)
{
	for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
	{
        // ��������
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability.GetDefaultObject(), 1);

        if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
        {
            // �����AuraGameplayAbility����ô����StartupInputTag
            AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
            // DynamicAbilityTags��һ����ǩ���ϣ���������һЩ��ǩ����Щ��ǩ����������ʱ��̬��ӻ��Ƴ���
            // ������������У�DynamicAbilityTags��AbilitySpec��һ����Ա���������ڴ洢��Ability��صı�ǩ��
            // ����˵�����Ǳ������ڿ�ʼ��ʱ���һ����ɫ�����һ������������������Ա������������������ҽ���������ǩ��ӵ������������DynamicAbilityTags�С�
            // Ȼ������Ϸ���й����У��ҿ���ж�ػ��߸�����������ǩ����Ϊ�Ҽ������ǩ��������������Ϳ��Ա��Ҽ���������ˡ�

            // ��������ӵ�AbilitySystemComponent��
            GiveAbility(AbilitySpec);   // �������
        }

        // GiveAbilityAndActivateOnce(AbilitySpec);    // ��Ӳ���������
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
    // 1���������ı�ǩ�Ƿ���Ч
    if (InputTag.IsValid())
    {
    	// 2����ȡ���еļ��������(�����������ָ��Щ���Ա����������) ��ȡ����AbilitySystemComponent�ļ������
		TArray<FGameplayAbilitySpec> AbilitiesOfTheActivatable = GetActivatableAbilities();

		// 3���������еļ��������
		for (FGameplayAbilitySpec& Spec : AbilitiesOfTheActivatable)
		{
            // 4����������������ǩ�Ƿ�������ı�ǩ��ͬ��������õ��Ǿ�ȷƥ��
            if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
            {
               // 5�� �ж������Ƿ��Ѿ�����
                if (!Spec.IsActive())
                {
                	// 6�����Լ�������
					TryActivateAbility(Spec.Handle);    // ���Լ�������
				}
            }
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
    // 1���������ı�ǩ�Ƿ���Ч
    if (InputTag.IsValid())
    {
        // 2����ȡ���еļ��������(�����������ָ��Щ���Ա����������) ��ȡ����AbilitySystemComponent�ļ������

        // 3���������еļ��������
        for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
        {
            // 4����������������ǩ�Ƿ�������ı�ǩ��ͬ��������õ��Ǿ�ȷƥ��
            if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
            {
				// 5���ͷ������ǩ
                AbilitySpecInputReleased(Spec);    // �ͷ������ǩ
            }
        }
    }
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
    // 1���������ı�ǩ�Ƿ���Ч
    if (InputTag.IsValid())
    {
        // 2����ȡ���еļ��������(�����������ָ��Щ���Ա����������) ��ȡ����AbilitySystemComponent�ļ������
        TArray<FGameplayAbilitySpec> AbilitiesOfTheActivatable = GetActivatableAbilities();

        // 3���������еļ��������
        for (FGameplayAbilitySpec& Spec : AbilitiesOfTheActivatable)
        {
            // 4����������������ǩ�Ƿ�������ı�ǩ��ͬ��������õ��Ǿ�ȷƥ��
            if (Spec.DynamicAbilityTags.HasTagExact(InputTag))
            {
				AbilitySpecInputPressed(Spec);      // ��ס�����ǩ
                // 5�� �ж������Ƿ��Ѿ�����
                if (!Spec.IsActive())
                {
                    // 6�����Լ�������
                    TryActivateAbility(Spec.Handle);    // ���Լ�������
                }
            }
        }
    }
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
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
