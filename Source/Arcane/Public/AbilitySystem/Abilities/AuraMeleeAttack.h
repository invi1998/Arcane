// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraMeleeAttack : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:
	// 传入Socket名称，传入是否是武器插槽或者是角色骨骼插槽，传入socket位置偏移量（默认为0），传入检测点数量（默认为1），传入检测长度（默认为100），生成一组检测点用于攻击检测
	UFUNCTION(BlueprintCallable, Category = "Melee Attack")
	TArray<FVector> GenerateAttackTraceSockets(const FName& SocketName, bool bWeapon, float ForwardOffset = 0.f, int32 Num = 1, float TotalLength = 100.0f);
	
};
