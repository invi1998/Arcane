// Copyright INVI1998


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorDef);	// 捕获护甲
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorPenetrationDef);	// 捕获穿甲
	RelevantAttributesToCapture.Add(AuraDamageStatics().BlockChanceDef);	// 捕获格挡
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 获取AbilitySystemComponent
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// 获取Avatar
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// 获取GameplayEffect
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// 通过Caller Magnitude（调用者的属性值）获取伤害
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	
	// 捕获格挡(受击者格挡几率)
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	// 捕获护甲(受击者护甲)
	float TargetArmor = 0.f;	// 捕获护甲
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);

	// 捕获穿甲 (攻击者穿甲) 百分比穿甲
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	// 判断是否格挡（百分比）
	if (FMath::RandRange(0, 100) <= TargetBlockChance)
	{
		// 伤害减少
		Damage *= 0.5f;
	}

	const float EffectTargetArmor = TargetArmor *= (100 - SourceArmorPenetration * 0.25f) / 100.f;	// 穿甲减少护甲

	// 最终伤害 = 格挡后的伤害 - 护甲 + 穿甲
	Damage = FMath::Max(0.f, Damage * (100 - EffectTargetArmor) / 100.f);

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
