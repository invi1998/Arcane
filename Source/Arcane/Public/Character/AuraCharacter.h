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

	virtual void PossessedBy(AController* NewController) override;		// 当角色被控制器控制时调用
	virtual void OnRep_PlayerState() override;		// 当角色的PlayerState被复制时调用

	/* Combat Interface */
	virtual int32 GetCharacterLevel_Implementation() const override;	// 获取等级
	/* Combat Interface End*/

	/* Player Interface Start*/
	void AddToEXP_Implementation(int32 EXP) override;	// 添加经验
	void LevelUp_Implementation(int32 Lv) override;	// 升级
	/* Player Interface End*/


private:
	virtual void InitAbilityActorInfo() override;	// 初始化能力Actor信息
	
};
