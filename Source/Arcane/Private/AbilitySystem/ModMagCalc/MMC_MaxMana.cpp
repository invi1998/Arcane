// Copyright INVI1998


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// Ϊ������Ҫ��������Դ���һ�����Զ��壬��Ϊ���ǵ����η����������������ֵ�ģ��������ȡ��������������������Ҫ������������
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 1: Cather tags from source and target ������Դ��Ŀ���в����ǩ��
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	//2: Create EvaluationParameters ����������������
	FAggregatorEvaluateParameters EvaluationParameters;	// ��������
	EvaluationParameters.SourceTags = SourceTags;	// ��Դ��ǩ
	EvaluationParameters.TargetTags = TargetTags;	// Ŀ���ǩ

	// 3: Get Captured Attribute Magnitude ����ȡ�����������ֵ��
	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);

	// 4: Clamp the value to be positive ����ֵ����Ϊ������
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	// 5: get the player level ����ȡ��ҵȼ���
	const ICombatInterface* SourceActor = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject());
	const int32 Level = SourceActor ? SourceActor->GetCharacterLevel() : 1;

	// 6��Return the max mana �����������ֵ��
	return 50.f + Level * 15.5f + Intelligence * 2.5f;
}
