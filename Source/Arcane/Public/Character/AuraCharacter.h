// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraCharacter : public AAuraCharacterBase, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;		// ����ɫ������������ʱ����
	virtual void OnRep_PlayerState() override;		// ����ɫ��PlayerState������ʱ����

	/* Combat Interface */
	virtual int32 GetPlayerLevel() const override;	// ��ȡ�ȼ�
	/* Combat Interface End*/

private:
	virtual void InitAbilityActorInfo() override;	// ��ʼ������Actor��Ϣ
	
};
