// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

/**
 * ���Լ�
 */
UCLASS()
class ARCANE_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	// ��дGetLifetimeReplicatedProps�����������������Եĸ��Ʒ�ʽ

	/** ����ֵ */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category="Vital Attributes")	// ��ͼֻ��������ʹ��OnRep_Health����������ΪVital Attributes����Ҫ���ԣ�
	FGameplayAttributeData Health;		// ����ֵ������ΪFGameplayAttributeData������һ���ṹ�壬�����˵�ǰֵ�ͻ���ֵ

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Vital Attributes")	// ��ͼֻ��������ΪVital Attributes����Ҫ���ԣ�
	FGameplayAttributeData MaxHealth;	// �������ֵ

	/** ħ�� */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Mana Attributes")	// ��ͼֻ��, ����ʹ��OnRep_Mana����������ΪMana Attributes��ħ�����ԣ�
	FGameplayAttributeData Mana;		// ħ��

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Mana Attributes")	// ��ͼֻ��������ΪMana Attributes��ħ�����ԣ�
	FGameplayAttributeData MaxMana;		// ���ħ��

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;		// һ����˵��Rep�����ǲ���Ҫ��������ģ������������Ҫ��Rep������ʹ�þ�ֵ����ô�����Ҫ�������

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;	// �������ֵ�ص�����

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;		// ħ���ص�����

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;	// ���ħ���ص�����
};
