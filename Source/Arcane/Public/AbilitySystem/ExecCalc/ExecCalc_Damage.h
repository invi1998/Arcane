// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
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
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);	// ��������
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);	// �����˺�
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);	// ��������

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);	// ���濹��
	DECLARE_ATTRIBUTE_CAPTUREDEF(IceResistance);	// ���俹��
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);	// ���翹��
	DECLARE_ATTRIBUTE_CAPTUREDEF(PoisonResistance);	// ���ؿ���
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);	// ��������
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);	// ������

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDef;		// ���ڴ洢���������

	AuraDamageStatics()
	{
		// �ú������ڲ�������
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);	// ����Ŀ��Ļ������ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);	// ������Դ�Ļ��״�͸���ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);		// ����Ŀ��ĸ񵲼������ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);	// ������Դ�ı����������ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);	// ������Դ�ı����˺����ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);	// ����Ŀ��ı����������ԣ����Ŀ���

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);	// ����Ŀ��Ļ��濹�����ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, IceResistance, Target, false);	// ����Ŀ��ĺ��俹�����ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);	// ����Ŀ������翹�����ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PoisonResistance, Target, false);	// ����Ŀ��Ķ��ؿ������ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);	// ����Ŀ��İ����������ԣ����Ŀ���
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);	// ����Ŀ������������ԣ����Ŀ���

		const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

		TagsToCaptureDef.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

		TagsToCaptureDef.Add(Tags.Attributes_Secondary_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_Resistance_Ice, IceResistanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_Resistance_Lightning, LightningResistanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_Resistance_Poison, PoisonResistanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDef.Add(Tags.Attributes_Secondary_Resistance_Physical, PhysicalResistanceDef);

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
