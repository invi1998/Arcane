// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;		// ����ɫ������������ʱ����
	virtual void OnRep_PlayerState() override;		// ����ɫ��PlayerState������ʱ����

	/* Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override;	// ��ȡ�ȼ�
	/* Combat Interface End*/

	/* Player Interface Start*/
	void AddToEXP_Implementation(int32 EXP) override;	// ��Ӿ���
	void LevelUp_Implementation(int32 Lv) override;	// ����
	/* Player Interface End*/


private:
	virtual void InitAbilityActorInfo() override;	// ��ʼ������Actor��Ϣ
	
};
