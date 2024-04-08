// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;		// ����ɫ������������ʱ����
	virtual void OnRep_PlayerState() override;		// ����ɫ��PlayerState������ʱ����

private:
	virtual void InitAbilityActorInfo() override;	// ��ʼ������Actor��Ϣ
	
};
