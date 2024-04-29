// Copyright INVI1998


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();		// 体质
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;	// 目标
	VigorDef.bSnapshot = false;	// 不是快照，快照是指在效果生效时捕获的属性值，而不是在效果创建时捕获的属性值，所以效果被创建时，属性值是多少，效果就是多少，这里获取的是真实值

	// 对体质（活力）定义设置了这些基础属性后，我们的修饰符计算还需要一个变量，他是要捕获的属性数值之一
	// 我们需要将这个 VigorDef 添加到 CapturedAttributeModifiers 数组中，这样我们的修饰符计算就可以获取到这个属性的数值了
	RelevantAttributesToCapture.Add(VigorDef);	// 添加到捕获属性数组中
	// 这样，在最终执行修饰符计算时，我们就可以获取到体质的数值了
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Cather tags from source and target （从来源和目标中捕获标签）
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();		// 来源标签
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();		// 目标标签

	// 现在，为了捕获一个属性并获取他的数值，我们需要使用 FAggregatorEvaluateParameters 结构体, 这个结构体包含了我们需要的所有信息
	FAggregatorEvaluateParameters EvaluationParameters;	// 评估参数
	EvaluationParameters.SourceTags = SourceTags;	// 来源标签
	EvaluationParameters.TargetTags = TargetTags;	// 目标标签

	float Vigor = 0.f;	// 体质
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);	// 获取捕获的属性数值，传入捕获定义，效果规格，评估参数，体质（属性）

	// Clamp the value to be positive （将值限制为正数）
	Vigor = FMath::Max<float>(Vigor, 0.f);	// 体质

	// 同时，我们希望玩家的最大生命值不仅取决于体质（vigor），还取决于其他因素，比如等级，所以我们需要一个公式来计算最大生命值
	// 这也是我们创建这个修饰符的原因，因为我们在这里可以始终获取到这个效果的来源和目标，所以我们可以根据来源和目标的属性来计算最大生命值
	int32 Level = 1;
	if (Spec.GetContext().GetSourceObject() && Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		if (const ICombatInterface* SourceActor = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject()))
		{
			Level = SourceActor->Execute_GetCharacterLevel(Spec.GetEffectContext().GetSourceObject());	// 获取角色等级
		}
	}
	return 80.f + Level * 10.5f + Vigor * 2.5f;	// 返回最大生命值

}
