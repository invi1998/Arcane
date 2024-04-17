// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	// 获取生命周期复制属性

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;		// 获取能力系统组件
	UAttributeSet* GetAttributeSet() const;												// 获取属性集

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }		// 获取等级

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;		// 能力系统组件

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;							// 属性集

private:
	UPROPERTY(ReplicatedUsing= OnRep_Level, EditAnywhere, Category="Attributes")
	int32 Level = 40;		// 玩家等级

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);	// 当玩家等级被复制时调用

};
