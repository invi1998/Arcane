// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

FString UAuraPassiveAbility::GetDescription(int32 Level)
{
	// 回血回蓝被动技能
	// 例如：每秒回复10点生命值和5点法力值

	const FAuraGameplayTags AuraTags = FAuraGameplayTags::Get();

	const float Value = PassiveValue.GetValueAtLevel(Level);

	FString Desc;

	if (AbilityTags.HasTagExact(AuraTags.Abilities_Passive_LifeSiphon))
	{
		Desc = FString::Printf(TEXT("<Title>生命虹吸</>\t<Small>（被动技能）</>\n\n"
			"\t<Default>每秒回复</><Chance>%.0f</><Default>点生命值</>\n"), Value);
	}
	else if (AbilityTags.HasTagExact(AuraTags.Abilities_Passive_ManaSiphon))
	{
		Desc = FString::Printf(TEXT("<Title>法力虹吸</>\t<Small>（被动技能）</>\n\n"
			"\t<Default>每秒回复</><Chance>%.0f</><Default>点法力值</>\n"), Value);
	}
	else if (AbilityTags.HasTagExact(AuraTags.Abilities_Passive_HaloOfProtection))
	{
		Desc = FString::Printf(TEXT("<Title>守护光环</>\t<Small>（被动技能）</>\n\n"
			"\t<Default>抵挡</><Damage>%.0f</><Default>点伤害</>\n"), Value);
	}

	return Desc;

}

FString UAuraPassiveAbility::GetNextLevelDescription(int32 Level)
{
	return GetDescription(Level + 1);
}

void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 绑定被动失效委托

	if (UAuraAbilitySystemComponent* AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraAbilitySystemComponent->DeactivatePassiveAbilitiesDelegate.AddUObject(this, &UAuraPassiveAbility::ReceiveDeactive);
	}

}

void UAuraPassiveAbility::ReceiveDeactive(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
