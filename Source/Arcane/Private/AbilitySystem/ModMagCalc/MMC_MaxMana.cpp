// Copyright INVI1998


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// 为我们想要捕获的属性创建一个属性定义，因为我们的修饰符是用来计算最大法力值的，而最大法力取决于智力，所以我们需要捕获智力属性
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 1: Cather tags from source and target （从来源和目标中捕获标签）
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//2: Create EvaluationParameters （创建评估参数）
	FAggregatorEvaluateParameters EvaluationParameters;	// 评估参数
	EvaluationParameters.SourceTags = SourceTags;	// 来源标签
	EvaluationParameters.TargetTags = TargetTags;	// 目标标签

	// 3: Get Captured Attribute Magnitude （获取捕获的属性数值）
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);

	// 4: Clamp the value to be positive （将值限制为正数）
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	// 5: get the player level （获取玩家等级）
	int32 Level = 1;
	if (Spec.GetContext().GetSourceObject() && Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		if (const ICombatInterface* SourceActor = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject()))
		{
			Level = SourceActor->Execute_GetCharacterLevel(Spec.GetEffectContext().GetSourceObject());
		}
	}

	// 6：Return the max mana （返回最大法力值）
	return 50.f + Level * 25.5f + Intelligence * 12.5f;
}
