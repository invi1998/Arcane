// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "PassiveNiagaraComponent.generated.h"

class UAuraAbilitySystemComponent;

/**
 * 被动技能的Niagara组件
 */
UCLASS()
class ARCANE_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UPassiveNiagaraComponent();


	UPROPERTY(EditDefaultsOnly, Category = "Niagara")
	FGameplayTag PassiveSpellTag;

protected:
	virtual void BeginPlay() override;

	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);	// 当被动技能激活或者失效时的委托回调

	void OnOwnerDeath(AActor* Actor);	// 当Owner死亡时的委托回调

	void ActivateNiagara(UAuraAbilitySystemComponent* AuraASC);	// 激活Niagara
};

