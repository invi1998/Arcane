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

	virtual void PossessedBy(AController* NewController) override;		// 当角色被控制器控制时调用
	virtual void OnRep_PlayerState() override;		// 当角色的PlayerState被复制时调用

	/* Combat Interface */
	virtual int32 GetPlayerLevel() const override;	// 获取等级
	/* Combat Interface End*/

private:
	virtual void InitAbilityActorInfo() override;	// 初始化能力Actor信息
	
};
