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
	
	// 捕获格挡(敌人格挡几率)
	float TargetBlockChance = UAuraAttributeSet().GetBlockChance();
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	float Armor = UAuraAttributeSet().GetArmor();	// 捕获护甲
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max(0.f, Armor);

	// 捕获穿甲
	float ArmorPenetration = UAuraAttributeSet().GetArmorPenetration();
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorPenetrationDef, EvaluationParameters, ArmorPenetration);
	ArmorPenetration = FMath::Max(0.f, FMath::Min(Armor, ArmorPenetration));

	// 判断是否格挡（百分比）
	if (FMath::RandRange(0, 100) <= TargetBlockChance)
	{
		// 伤害减少
		Damage *= 0.5f;
	}

	// 最终伤害 = 格挡后的伤害 - 护甲 + 穿甲
	Damage = FMath::Max(0.f, Damage + ArmorPenetration - Armor);

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
