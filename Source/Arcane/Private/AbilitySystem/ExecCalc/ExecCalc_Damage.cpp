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

	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	// 获取等级
	const int32 SourceLevel = SourceCombatInterface ? SourceCombatInterface->GetPlayerLevel() : 1;
	const int32 TargetLevel = TargetCombatInterface ? TargetCombatInterface->GetPlayerLevel() : 1;

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

	// 捕获穿甲 (攻击者穿甲) 百分比穿甲
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	

	// 判断是否格挡（百分比）
	if (FMath::RandRange(0, 100) <= TargetBlockChance)
	{
		// 伤害减少
		Damage *= 0.5f;
	}

	const UCharacterClassInfo* SourceClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	// const UCharacterClassInfo* TargetClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);

	const FRealCurve* ArmorPenetrationCurve = SourceClassInfo->DamageCalculationCurveTable->FindCurve(FName("ArmorPenetration"), FString());
	const float TargetArmorReal = ArmorPenetrationCurve ? ArmorPenetrationCurve->Eval(TargetLevel) : 1.f;
	TargetArmor *= (1 - TargetArmorReal);	// 护甲减少
	TargetArmor = FMath::Max(0.f, TargetArmor);

	// UKismetSystemLibrary::PrintString(SourceAvatar, FString::Printf(TEXT("TargetArmor: %f"), TargetArmor), true, false, FLinearColor::Red, 1.f);

	const FRealCurve* ArmorEffectCurve = SourceClassInfo->DamageCalculationCurveTable->FindCurve(FName("EffectiveArmor"), FString());
	const float SourceArmorPenetrationRate = ArmorEffectCurve ? ArmorEffectCurve->Eval(SourceLevel) : 1.f;
	SourceArmorPenetration *= (1 - SourceArmorPenetrationRate);	// 穿甲减少
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	const float EffectTargetArmor =  TargetArmor * (100 - SourceArmorPenetration) / 100.f;	// 效果护甲

	// 最终伤害 = 格挡后的伤害 - 护甲 + 穿甲
	Damage = FMath::Max(0.f, Damage * (100 - EffectTargetArmor) / 100.f);

	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);

}
