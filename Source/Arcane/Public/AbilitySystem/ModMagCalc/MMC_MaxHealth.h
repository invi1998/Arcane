// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxHealth();

	// 这个函数是用来计算最终的数值的，修饰符
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// 为我们想要捕获的属性创建一个属性定义
	FGameplayEffectAttributeCaptureDefinition VigorDef;		// 体质
};
