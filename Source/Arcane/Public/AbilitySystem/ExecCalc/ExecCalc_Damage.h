// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "ExecCalc_Damage.generated.h"

// 该结构体用于存储捕获的属性（该结构体作为一个原生结构体，不会用于蓝图）
struct AuraDamageStatics
{
	// 该宏用于声明一个捕获属性的结构体，其中包含了捕获的属性的指针，以及捕获的属性的名称
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);		// 护甲
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);	// 护甲穿透
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);	// 格挡几率
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);	// 暴击几率
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);	// 暴击伤害
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);	// 暴击抗性

	AuraDamageStatics()
	{
		// 该函数用于捕获属性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);	// 捕获目标的护甲属性，不拍快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);	// 捕获来源的护甲穿透属性，不拍快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);		// 捕获目标的格挡几率属性，不拍快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);	// 捕获来源的暴击几率属性，不拍快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);	// 捕获来源的暴击伤害属性，不拍快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);	// 捕获目标的暴击抗性属性，不拍快照
	}
};

// 该静态函数将在每次调用时返回一个AuraDamageStatics的实例，返回值每次调用都是相同的
static const AuraDamageStatics& GetAuraDamageStatics()
{
	static AuraDamageStatics AuraStatics;
	return AuraStatics;
}

/**
 * 
 */
UCLASS()
class ARCANE_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	// 该函数用于计算伤害，
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
