// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

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
	virtual void AddToEXP_Implementation(int32 EXP) override;	// ��Ӿ���
	virtual void SetEXP_Implementation(int32 EXP) override;	// ���þ���
	virtual void LevelUp_Implementation(int32 Lv) override;	// ����
	virtual void SetLevel_Implementation(int32 Lv) override;	// ���õȼ�
	virtual int32 GetEXP_Implementation() const override;	// ��ȡ����
	virtual int32 GetLevelByEXP_Implementation(int32 EXP) const override;	// ͨ�������ȡ�ȼ�
	virtual int32 GetAttributePointReward_Implementation() const override;	// ��ȡ���Ե㽱��
	virtual int32 GetSkillPointReward_Implementation() const override;	// ��ȡ���ܵ㽱��
	virtual void AddAttributePoint_Implementation(int32 Point) override;	// ���Լӵ�
	virtual void AddSkillPoint_Implementation(int32 Point) override;	// ���ܼӵ�
	virtual int32 GetAttributePoint_Implementation() const override;	// ��ȡ���Ե�
	virtual int32 GetSkillPoint_Implementation() const override;	// ��ȡ���ܵ�
	/* Player Interface End*/

	// ��ɫ������Ч
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraComponent> LevelUpEffect;


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;	// ����������

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;	// ���ҡ��

	virtual void InitAbilityActorInfo() override;	// ��ʼ������Actor��Ϣ

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpEffect() const;	// �ಥ������Ч
	
};
