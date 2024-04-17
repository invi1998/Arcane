// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "ExecCalc_Damage.generated.h"

// �ýṹ�����ڴ洢��������ԣ��ýṹ����Ϊһ��ԭ���ṹ�壬����������ͼ��
struct AuraDamageStatics
{
	// �ú���������һ���������ԵĽṹ�壬���а����˲�������Ե�ָ�룬�Լ���������Ե�����
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);		// ����
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);	// ���״�͸
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);	// �񵲼���

	AuraDamageStatics()
	{
		// �ú������ڲ�������
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);	// ����Ŀ��Ļ������ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);	// ������Դ�Ļ��״�͸���ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);		// ����Ŀ��ĸ񵲼������ԣ����Ŀ���
	}
};

// �þ�̬��������ÿ�ε���ʱ����һ��AuraDamageStatics��ʵ��������ֵÿ�ε��ö�����ͬ��
static const AuraDamageStatics& GetAuraDamageStatics()
{
	static AuraDamageStatics AuraStatics;
	return AuraStatics;
}

/**
 * 
 */
UCLASS()
class ARCANE_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();

	// �ú������ڼ����˺���
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};
