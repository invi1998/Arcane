// Copyright INVI1998


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();		// ����
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;	// Ŀ��
	VigorDef.bSnapshot = false;	// ���ǿ��գ�������ָ��Ч����Чʱ���������ֵ����������Ч������ʱ���������ֵ������Ч��������ʱ������ֵ�Ƕ��٣�Ч�����Ƕ��٣������ȡ������ʵֵ

	// �����ʣ�������������������Щ�������Ժ����ǵ����η����㻹��Ҫһ������������Ҫ�����������ֵ֮һ
	// ������Ҫ����� VigorDef ��ӵ� CapturedAttributeModifiers �����У��������ǵ����η�����Ϳ��Ի�ȡ��������Ե���ֵ��
	RelevantAttributesToCapture.Add(VigorDef);	// ��ӵ���������������
	// ������������ִ�����η�����ʱ�����ǾͿ��Ի�ȡ�����ʵ���ֵ��
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Cather tags from source and target ������Դ��Ŀ���в����ǩ��
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();		// ��Դ��ǩ
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();		// Ŀ���ǩ

	// ���ڣ�Ϊ�˲���һ�����Բ���ȡ������ֵ��������Ҫʹ�� FAggregatorEvaluateParameters �ṹ��, ����ṹ�������������Ҫ��������Ϣ
	FAggregatorEvaluateParameters EvaluationParameters;	// ��������
	EvaluationParameters.SourceTags = SourceTags;	// ��Դ��ǩ
	EvaluationParameters.TargetTags = TargetTags;	// Ŀ���ǩ

	float Vigor = 0.f;	// ����
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);	// ��ȡ�����������ֵ�����벶���壬Ч������������������ʣ����ԣ�

	// Clamp the value to be positive ����ֵ����Ϊ������
	Vigor = FMath::Max<float>(Vigor, 0.f);	// ����

	// ͬʱ������ϣ����ҵ��������ֵ����ȡ�������ʣ�vigor������ȡ�����������أ�����ȼ�������������Ҫһ����ʽ�������������ֵ
	// ��Ҳ�����Ǵ���������η���ԭ����Ϊ�������������ʼ�ջ�ȡ�����Ч������Դ��Ŀ�꣬�������ǿ��Ը�����Դ��Ŀ��������������������ֵ
	int32 Level = 1;
	if (Spec.GetContext().GetSourceObject() && Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		if (const ICombatInterface* SourceActor = Cast<ICombatInterface>(Spec.GetEffectContext().GetSourceObject()))
		{
			Level = SourceActor->Execute_GetCharacterLevel(Spec.GetEffectContext().GetSourceObject());	// ��ȡ��ɫ�ȼ�
		}
	}
	return 80.f + Level * 10.5f + Vigor * 2.5f;	// �����������ֵ

}
