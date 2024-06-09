// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UDebuffNiagaraComponent();

	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;		// 减益Buff的Tag

protected:
	// 响应减益Buff的Tag，激活Niagara组件或者停止Niagara组件

	virtual void BeginPlay() override;

	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);	// 减益Buff的Tag发生变化时的回调函数

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);	// 当Owner死亡时的回调函数
	
};
