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
	int32 GetEXP_Implementation() const override;	// ��ȡ����
	int32 GetLevelByEXP_Implementation(int32 EXP) const override;	// ͨ�������ȡ�ȼ�
	int32 GetAttributePointReward_Implementation() const override;	// ��ȡ���Ե㽱��
	int32 GetSkillPointReward_Implementation() const override;	// ��ȡ���ܵ㽱��
	void AddAttributePoint_Implementation(int32 Point) override;	// ���Լӵ�
	void AddSkillPoint_Implementation(int32 Point) override;	// ���ܼӵ�
	/* Player Interface End*/


private:
	virtual void InitAbilityActorInfo() override;	// ��ʼ������Actor��Ϣ
	
};
