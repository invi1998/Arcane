// Copyright INVI1998


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorDef);	// ���񻤼�
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArmorPenetrationDef);	// ���񴩼�
	RelevantAttributesToCapture.Add(AuraDamageStatics().BlockChanceDef);	// �����
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

	// ͨ��Caller Magnitude�������ߵ�����ֵ����ȡ�˺�
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	
	// �����(���˸񵲼���)
	float TargetBlockChance = UAuraAttributeSet().GetBlockChance();
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	float Armor = UAuraAttributeSet().GetArmor();	// ���񻤼�
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max(0.f, Armor);

	// ���񴩼�
	float ArmorPenetration = UAuraAttributeSet().GetArmorPenetration();
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorPenetrationDef, EvaluationParameters, ArmorPenetration);
	ArmorPenetration = FMath::Max(0.f, FMath::Min(Armor, ArmorPenetration));

	// �ж��Ƿ�񵲣��ٷֱȣ�
	if (FMath::RandRange(0, 100) <= TargetBlockChance)
	{
		// �˺�����
		Damage *= 0.5f;
	}

	// �����˺� = �񵲺���˺� - ���� + ����
	Damage = FMath::Max(0.f, Damage + ArmorPenetration - Armor);

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
