// Copyright INVI1998


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorDef);	// ���񻤼�
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// ��ȡAbilitySystemComponent
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// ��ȡAvatar
	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	// ��ȡGameplayEffect
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.f;

	// ��ȡ����ֵ
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		AuraDamageStatics().ArmorDef,
		EvaluationParameters,
		Armor
		);

	Armor = FMath::Max(0.f, Armor);

	float ArmorPenetration = 0.0f;	// ���״�͸
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		AuraDamageStatics().ArmorPenetrationDef,
		EvaluationParameters,
		ArmorPenetration
		);

	// ���������ߣ�Ҳ��ֻ�ܴ�͸100%�Ļ���
	ArmorPenetration = FMath::Max(0.f, FMath::Min(Armor, ArmorPenetration));

	FGameplayModifierEvaluatedData DamageData(AuraDamageStatics().ArmorProperty, EGameplayModOp::Additive, -Armor);
	FGameplayModifierEvaluatedData ArmorPenetrationData(AuraDamageStatics().ArmorPenetrationProperty, EGameplayModOp::Additive, ArmorPenetration);

	// �����˺� ��ȥ����
	OutExecutionOutput.AddOutputModifier(
		DamageData
	);

	// ���ϻ��״�͸
	OutExecutionOutput.AddOutputModifier(
		ArmorPenetrationData
	);

}
