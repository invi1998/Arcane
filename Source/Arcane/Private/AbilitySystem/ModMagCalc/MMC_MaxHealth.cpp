// Copyright INVI1998


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// ��ȡ���ʵ���ֵ
	float Vigor = 0.f;
	// ��Spec�л�ȡ���ʵ���ֵ
	Spec.CapturedAttributeModifiers.FindCaptureFloat(VigorDef, Vigor);

	// �������յ���ֵ
	return Vigor * 10.f;
}
