// Copyright INVI1998


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 获取体质的数值
	float Vigor = 0.f;
	// 从Spec中获取体质的数值
	Spec.CapturedAttributeModifiers.FindCaptureFloat(VigorDef, Vigor);

	// 计算最终的数值
	return Vigor * 10.f;
}
