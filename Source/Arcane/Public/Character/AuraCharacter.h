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

	virtual void PossessedBy(AController* NewController) override;		// 当角色被控制器控制时调用
	virtual void OnRep_PlayerState() override;		// 当角色的PlayerState被复制时调用

	/* Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override;	// 获取等级
	/* Combat Interface End*/

	/* Player Interface Start*/
	void AddToEXP_Implementation(int32 EXP) override;	// 添加经验
	void SetEXP_Implementation(int32 EXP) override;	// 设置经验
	void LevelUp_Implementation(int32 Lv) override;	// 升级
	void SetLevel_Implementation(int32 Lv) override;	// 设置等级
	int32 GetEXP_Implementation() const override;	// 获取经验
	int32 GetLevelByEXP_Implementation(int32 EXP) const override;	// 通过经验获取等级
	int32 GetAttributePointReward_Implementation() const override;	// 获取属性点奖励
	int32 GetSkillPointReward_Implementation() const override;	// 获取技能点奖励
	void AddAttributePoint_Implementation(int32 Point) override;	// 属性加点
	void AddSkillPoint_Implementation(int32 Point) override;	// 技能加点
	/* Player Interface End*/

	// 角色升级特效
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraComponent> LevelUpEffect;


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;	// 顶部相机组件

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;	// 相机摇臂

	virtual void InitAbilityActorInfo() override;	// 初始化能力Actor信息

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpEffect() const;	// 多播升级特效
	
};
