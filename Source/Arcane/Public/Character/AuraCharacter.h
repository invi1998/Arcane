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

	virtual void PossessedBy(AController* NewController) override;		// 当角色被控制器控制时调用
	virtual void OnRep_PlayerState() override;		// 当角色的PlayerState被复制时调用

private:
	virtual void InitAbilityActorInfo() override;	// 初始化能力Actor信息
	
};
