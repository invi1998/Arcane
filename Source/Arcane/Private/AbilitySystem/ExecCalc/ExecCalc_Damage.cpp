// Copyright INVI1998


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().ArmorDef);	// 捕获护甲
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().ArmorPenetrationDef);	// 捕获穿甲
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().BlockChanceDef);	// 捕获格挡
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().CriticalHitChanceDef);	// 暴击几率
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().CriticalHitDamageDef);	// 暴击伤害
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().CriticalHitResistanceDef);	// 暴击抗性
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().FireResistanceDef);	// 火焰抗性
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().IceResistanceDef);	// 寒冷抗性
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().LightningResistanceDef);	// 闪电抗性
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().PoisonResistanceDef);	// 毒素抗性
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().ArcaneResistanceDef);	// 奥术抗性
	RelevantAttributesToCapture.Add(GetAuraDamageStatics().PhysicalResistanceDef);	// 物理抗性

}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDef) const
{
	// 获取GameplayTag
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	// Debuff
	for (TTuple<FGameplayTag, FGameplayTag> Pair : AuraTags.DamageTypesToDebuff)	// key: 伤害类型 value: 伤害抗性类型
	{
		const FGameplayTag DebuffTag = Pair.Key;				// Debuff DamageType Tag
		const FGameplayTag DebuffResistanceTag = Pair.Value;	// Debuff Param Tag

		const float TypeDamage = Spec.GetSetByCallerMagnitude(DebuffTag, false, 0);	// 获取Debuff伤害

		if (TypeDamage > 0)
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(AuraTags.Debuff_Chance, false, 0);	// 获取Debuff几率

			float TargetDebuffResistance = 0.f;	// Debuff抗性
			const FGameplayTag& ResistanceTag = AuraTags.DamageTypesToResistance[DebuffTag];	// 获取Debuff抗性Tag
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDef[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Clamp(TargetDebuffResistance, 0.f, 100.f);	// 最小值为0
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;	// 有效Debuff几率
			const bool bDebuff = FMath::RandRange(0, 100) <= EffectiveDebuffChance;	// 是否Debuff
			if (bDebuff)
			{
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(AuraTags.Debuff_Damage, false, 0);	// Debuff伤害
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(AuraTags.Debuff_Frequency, false, 0);	// Debuff频率
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(AuraTags.Debuff_Duration, false, 0);	// Debuff持续时间

				// 设置Debuff参数在AuraGameplayEffectContext中
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();		// 获取GameplayEffectContextHandle

				UAuraAbilitySystemLibrary::SetSuccessfulDebuff(ContextHandle, true);	// 设置成功施加Debuff
				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);	// 设置Debuff伤害
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);	// 设置Debuff频率
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);	// 设置Debuff持续时间
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DebuffTag);	// 设置伤害类型
				
			}
		}
		
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 获取GameplayTag
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;		// 用于存储捕获的属性

	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_Armor, GetAuraDamageStatics().ArmorDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_ArmorPenetration, GetAuraDamageStatics().ArmorPenetrationDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_BlockChance, GetAuraDamageStatics().BlockChanceDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_CriticalHitChance, GetAuraDamageStatics().CriticalHitChanceDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_CriticalHitDamage, GetAuraDamageStatics().CriticalHitDamageDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_CriticalHitResistance, GetAuraDamageStatics().CriticalHitResistanceDef);

	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_Resistance_Fire, GetAuraDamageStatics().FireResistanceDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_Resistance_Ice, GetAuraDamageStatics().IceResistanceDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_Resistance_Lightning, GetAuraDamageStatics().LightningResistanceDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_Resistance_Poison, GetAuraDamageStatics().PoisonResistanceDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_Resistance_Arcane, GetAuraDamageStatics().ArcaneResistanceDef);
	TagsToCaptureDef.Add(AuraTags.Attributes_Secondary_Resistance_Physical, GetAuraDamageStatics().PhysicalResistanceDef);

	// 获取AbilitySystemComponent
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	// 获取Avatar
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	ICombatInterface* SourceCombatInterface = SourceAvatar->Implements<UCombatInterface>() ? Cast<ICombatInterface>(SourceAvatar) : nullptr;
	ICombatInterface* TargetCombatInterface = TargetAvatar->Implements<UCombatInterface>() ? Cast<ICombatInterface>(TargetAvatar) : nullptr;

	// 获取等级
	const int32 SourceLevel = SourceCombatInterface ? SourceCombatInterface->Execute_GetCharacterLevel(SourceAvatar) : 1;
	const int32 TargetLevel = TargetCombatInterface ? TargetCombatInterface->Execute_GetCharacterLevel(TargetAvatar) : 1;

	// 获取GameplayEffect
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();		// 获取GameplayEffectContextHandle

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	DetermineDebuff(ExecutionParams, Spec, EvaluationParameters, TagsToCaptureDef);	// 确定Debuff

	// 通过Caller Magnitude（调用者的属性值）获取伤害
	float Damage = 0;

	// key: 伤害类型 value: 伤害抗性类型
	for (const TTuple<FGameplayTag, FGameplayTag>& pair : AuraTags.DamageTypesToResistance)
	{
		const FGameplayTag DamageTypeTag = pair.Key;		// 伤害类型
		const FGameplayTag ResistanceTypeTag = pair.Value;	// 伤害抗性类型

		checkf(TagsToCaptureDef.Contains(ResistanceTypeTag), TEXT("ResistanceTypeTag not found in TagsToCaptureDef"));	// 检查伤害类型是否存在

		const FGameplayEffectAttributeCaptureDefinition DamageTypeCaptureDef = TagsToCaptureDef[ResistanceTypeTag];	// 获取捕获属性定义

		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTypeTag, false, 0);	// 获取伤害值，如果没有则返回0，同时不报警告和不打印日志

		float ResistanceValue = 0.f;	// 伤害抗性值
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageTypeCaptureDef, EvaluationParameters, ResistanceValue);	// 计算捕获属性值
		ResistanceValue = FMath::Clamp(ResistanceValue, 0.f, 100.f);	// 最小值为0

		// 我们希望伤害抗性是百分比，它将以百分比减少伤害
		DamageTypeValue *= (100 - ResistanceValue) / 100.f;


		// 检测是否是径向伤害类型，如果是，我们则需要针对Tagetd到伤害中心的距离依据径向伤害参数进行修正
		if (UAuraAbilitySystemLibrary::IsRadialDamage(ContextHandle))
		{
			//1.覆盖AuraCharacterBase中的TakeDamage。
			//2.创建委托0nDamageDelegate，在TakeDamage中接收广播Damage
			//3.将lambda绑定到受害人的OnDamageDelegate。
			//4.调用UGameplayStatics::ApplyRadialDamagelithfalloff造成伤害(这将调用受害者的TokeDamage)，然后广播OnDamageDelegate)
			//5.在Lambda，将DamageTypeValue设置为从广播中收到的Damage数值

			if (TargetCombatInterface)
			{
				TargetCombatInterface->GetOnDamageDelegate().AddLambda([&](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar, 
				DamageTypeValue, 
				0,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(ContextHandle), 
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(ContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(ContextHandle), 
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}

		Damage += DamageTypeValue;
	}

	const UCharacterClassInfo* SourceClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	// const UCharacterClassInfo* TargetClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(TargetAvatar);

	// 捕获暴击几率(攻击者暴击几率)
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);

	if (FMath::RandRange(0, 100) <= SourceCriticalHitChance)
	{
		UAuraAbilitySystemLibrary::SetCriticalHit(ContextHandle, true);	// 设置暴击
		// 暴击伤害
		float CriticalHitDamage = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().CriticalHitDamageDef, EvaluationParameters, CriticalHitDamage);
		CriticalHitDamage = FMath::Max(0.f, CriticalHitDamage);

		const FRealCurve* CriticalHitResistanceRateCurve = SourceClassInfo->DamageCalculationCurveTable->FindCurve(FName("CriticalHitResistance"), FString());
		const float CriticalHitResistanceRate = CriticalHitResistanceRateCurve ? CriticalHitResistanceRateCurve->Eval(TargetLevel) : 1.f;

		// 暴击抗性
		float CriticalHitResistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().CriticalHitResistanceDef, EvaluationParameters, CriticalHitResistance);

		const float EffectiveCriticalHitDamage = CriticalHitDamage * (1 - CriticalHitResistanceRate);

		Damage = EffectiveCriticalHitDamage + 2.f * Damage;
	}
	else
	{
		UAuraAbilitySystemLibrary::SetCriticalHit(ContextHandle, false);	// 设置非暴击
	}
	
	// 捕获格挡(受击者格挡几率)
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	// 捕获护甲(受击者护甲)
	float TargetArmor = 0.f;	// 捕获护甲
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().ArmorDef, EvaluationParameters, TargetArmor);

	// 捕获穿甲 (攻击者穿甲) 百分比穿甲
	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetAuraDamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	

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
