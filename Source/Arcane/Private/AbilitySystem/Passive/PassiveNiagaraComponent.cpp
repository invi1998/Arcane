// Copyright INVI1998


#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());

	// 绑定AbilitySystemComponent的激活被动技能的委托
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->ActivatePassiveEffectDelegate.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
		ActivateNiagara(AuraASC);
	}
	else if (CombatInterface)
	{
		// 如果ASC没有被创建，那么我们就需要监听ASC的创建委托，当ASC被创建时，我们就可以绑定激活被动技能的委托
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				AuraASC->ActivatePassiveEffectDelegate.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);

				ActivateNiagara(AuraASC);
			}
		});
	}

	// 同时，我们还需要监听角色死亡的委托，当角色死亡时，我们就可以停止Niagara组件
	if (CombatInterface)
	{
		CombatInterface->GetOnCharacterDeathDelegate().AddDynamic(this, &UPassiveNiagaraComponent::OnOwnerDeath);
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag != PassiveSpellTag)
	{
		return;
	}

	if (bActivate)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}

void UPassiveNiagaraComponent::OnOwnerDeath(AActor* Actor)
{
	Deactivate();
}

void UPassiveNiagaraComponent::ActivateNiagara(UAuraAbilitySystemComponent* AuraASC)
{
	if (const bool bStartupAbilitiesGiven = AuraASC->bStartupAbilitiesGiven)
	{
		if (AuraASC->GetStatusTagByAbilityTag(PassiveSpellTag) == FAuraGameplayTags::Get().Abilities_State_Equipped)
		{
			Activate();
		}
	}
}
