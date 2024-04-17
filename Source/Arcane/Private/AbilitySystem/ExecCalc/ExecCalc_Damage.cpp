// Copyright INVI1998


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

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

	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	// ��ȡ�ȼ�
	const int32 SourceLevel = SourceCombatInterface ? SourceCombatInterface->GetPlayerLevel() : 1;
	const int32 TargetLevel = TargetCombatInterface ? TargetCombatInterface->GetPlayerLevel() : 1;

	// ��ȡGameplayEffect
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// ͨ��Caller Magnitude�������ߵ�����ֵ����ȡ�˺�
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
	
	// �����(�ܻ��߸񵲼���)
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	// ���񻤼�(�ܻ��߻���)
	float TargetArmor = 0.f;	// ���񻤼�
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);

	// ���񴩼� (�����ߴ���) �ٷֱȴ���
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	

	// �ж��Ƿ�񵲣��ٷֱȣ�
	if (FMath::RandRange(0, 100) <= TargetBlockChance)
	{
		// �˺�����
		Damage *= 0.5f;
	}

	const UCharacterClassInfo* SourceClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	// const UCharacterClassInfo* TargetClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);

	const FRealCurve* ArmorPenetrationCurve = SourceClassInfo->DamageCalculationCurveTable->FindCurve(FName("ArmorPenetration"), FString());
	const float TargetArmorReal = ArmorPenetrationCurve ? ArmorPenetrationCurve->Eval(TargetLevel) : 1.f;
	TargetArmor *= (1 - TargetArmorReal);	// ���׼���
	TargetArmor = FMath::Max(0.f, TargetArmor);

	// UKismetSystemLibrary::PrintString(SourceAvatar, FString::Printf(TEXT("TargetArmor: %f"), TargetArmor), true, false, FLinearColor::Red, 1.f);

	const FRealCurve* ArmorEffectCurve = SourceClassInfo->DamageCalculationCurveTable->FindCurve(FName("EffectiveArmor"), FString());
	const float SourceArmorPenetrationRate = ArmorEffectCurve ? ArmorEffectCurve->Eval(SourceLevel) : 1.f;
	SourceArmorPenetration *= (1 - SourceArmorPenetrationRate);	// ���׼���
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	const float EffectTargetArmor =  TargetArmor * (100 - SourceArmorPenetration) / 100.f;	// Ч������

	// �����˺� = �񵲺���˺� - ���� + ����
	Damage = FMath::Max(0.f, Damage * (100 - EffectTargetArmor) / 100.f);

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
