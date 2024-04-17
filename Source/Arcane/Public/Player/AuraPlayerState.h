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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	// ��ȡ�������ڸ�������

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;		// ��ȡ����ϵͳ���
	UAttributeSet* GetAttributeSet() const;												// ��ȡ���Լ�

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }		// ��ȡ�ȼ�

protected:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;		// ����ϵͳ���

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;							// ���Լ�

private:
	UPROPERTY(ReplicatedUsing= OnRep_Level, EditAnywhere, Category="Attributes")
	int32 Level = 40;		// ��ҵȼ�

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);	// ����ҵȼ�������ʱ����

};
