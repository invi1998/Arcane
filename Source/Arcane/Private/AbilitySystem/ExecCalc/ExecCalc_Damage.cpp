// Copyright INVI1998


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
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
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitChanceDef);	// 暴击几率
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitDamageDef);	// 暴击伤害
	RelevantAttributesToCapture.Add(AuraDamageStatics().CriticalHitResistanceDef);	// 暴击抗性
	RelevantAttributesToCapture.Add(AuraDamageStatics().FireResistanceDef);	// 火焰抗性
	RelevantAttributesToCapture.Add(AuraDamageStatics().IceResistanceDef);	// 寒冷抗性
	RelevantAttributesToCapture.Add(AuraDamageStatics().LightningResistanceDef);	// 闪电抗性
	RelevantAttributesToCapture.Add(AuraDamageStatics().PoisonResistanceDef);	// 毒素抗性
	RelevantAttributesToCapture.Add(AuraDamageStatics().ArcaneResistanceDef);	// 奥术抗性
	RelevantAttributesToCapture.Add(AuraDamageStatics().PhysicalResistanceDef);	// 物理抗性

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

	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();		// 获取GameplayEffectContextHandle

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// 通过Caller Magnitude（调用者的属性值）获取伤害
	float Damage = 0;

	// key: 伤害类型 value: 伤害抗性类型
	for (const TTuple<FGameplayTag, FGameplayTag>& pair : FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		const FGameplayTag DamageTypeTag = pair.Key;		// 伤害类型
		const FGameplayTag ResistanceTypeTag = pair.Value;	// 伤害抗性类型

		checkf(AuraDamageStatics().TagsToCaptureDef.Contains(ResistanceTypeTag), TEXT("ResistanceTypeTag not found in TagsToCaptureDef"));	// 检查伤害类型是否存在

		const FGameplayEffectAttributeCaptureDefinition DamageTypeCaptureDef = AuraDamageStatics().TagsToCaptureDef[ResistanceTypeTag];	// 获取捕获属性定义

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false, 0);	// 获取伤害值，如果没有则返回0，同时不报警告和不打印日志

		float ResistanceValue = 0.f;	// 伤害抗性值
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageTypeCaptureDef, EvaluationParameters, ResistanceValue);	// 计算捕获属性值
		ResistanceValue = FMath::Clamp(ResistanceValue, 0.f, 100.f);	// 最小值为0

		// 我们希望伤害抗性是百分比，它将以百分比减少伤害
		DamageTypeValue *= (100 - ResistanceValue) / 100.f;

		Damage += DamageTypeValue;
	}

	const UCharacterClassInfo* SourceClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	// const UCharacterClassInfo* TargetClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);

	// 捕获暴击几率(攻击者暴击几率)
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);

	if (FMath::RandRange(0, 100) <= SourceCriticalHitChance)
	{
		UAuraAbilitySystemLibrary::SetCriticalHit(ContextHandle, true);	// 设置暴击
		// 暴击伤害
		float CriticalHitDamage = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().CriticalHitDamageDef, EvaluationParameters, CriticalHitDamage);
		CriticalHitDamage = FMath::Max(0.f, CriticalHitDamage);

		const FRealCurve* CriticalHitResistanceRateCurve = SourceClassInfo->DamageCalculationCurveTable->FindCurve(FName("CriticalHitResistance"), FString());
		const float CriticalHitResistanceRate = CriticalHitResistanceRateCurve ? CriticalHitResistanceRateCurve->Eval(TargetLevel) : 1.f;

		// 暴击抗性
		float CriticalHitResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AuraDamageStatics().CriticalHitResistanceDef, EvaluationParameters, CriticalHitResistance);

		const float EffectiveCriticalHitDamage = CriticalHitDamage * (1 - CriticalHitResistanceRate);

		Damage = EffectiveCriticalHitDamage + 2.f * Damage;
	}
	else
	{
		UAuraAbilitySystemLibrary::SetCriticalHit(ContextHandle, false);	// 设置非暴击
	}
	
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
		UAuraAbilitySystemLibrary::SetBlockedHit(ContextHandle, true);	// 设置格挡
		// 伤害减少
		Damage *= 0.5f;
	}
	else
	{
		UAuraAbilitySystemLibrary::SetBlockedHit(ContextHandle, false);	// 设置非格挡
	}

	const FRealCurve* ArmorPenetrationCurve = SourceClassInfo->DamageCalculationCurveTable->FindCurve(FName("ArmorPenetration"), FString());
	const float TargetArmorReal = ArmorPenetrationCurve ? ArmorPenetrationCurve->Eval(TargetLevel) : 1.f;
	TargetArmor *= (1 - TargetArmorReal);	// 护甲减少
	TargetArmor = FMath::Max(0.f, TargetArmor);

	// UKismetSystemLibrary::PrintString(SourceAvatar, FString::Printf(TEXT("TargetArmor: %f, TargetLevel: %d, SourceLevel: %d"), TargetArmor, TargetLevel, SourceLevel), true, false, FLinearColor::Red, 1.f);

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
