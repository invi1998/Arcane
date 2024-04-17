// Copyright INVI1998


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorDef);	// 捕获护甲
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

	float Armor = 0.f;

	// 获取属性值
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		AuraDamageStatics().ArmorDef,
		EvaluationParameters,
		Armor
		);

	Armor = FMath::Max(0.f, Armor);

	float ArmorPenetration = 0.0f;	// 护甲穿透
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		AuraDamageStatics().ArmorPenetrationDef,
		EvaluationParameters,
		ArmorPenetration
		);

	// 护穿叠过高，也最只能穿透100%的护甲
	ArmorPenetration = FMath::Max(0.f, FMath::Min(Armor, ArmorPenetration));

	FGameplayModifierEvaluatedData DamageData(AuraDamageStatics().ArmorProperty, EGameplayModOp::Additive, -Armor);
	FGameplayModifierEvaluatedData ArmorPenetrationData(AuraDamageStatics().ArmorPenetrationProperty, EGameplayModOp::Additive, ArmorPenetration);

	// 最终伤害 减去护甲
	OutExecutionOutput.AddOutputModifier(
		DamageData
	);

	// 加上护甲穿透
	OutExecutionOutput.AddOutputModifier(
		ArmorPenetrationData
	);

}
