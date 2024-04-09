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

	// ��������������������յ���ֵ�ģ����η�
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	// Ϊ������Ҫ��������Դ���һ�����Զ���
	FGameplayEffectAttributeCaptureDefinition VigorDef;		// ����
};
