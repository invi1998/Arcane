// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

// �ú������������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


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

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;	// ��дPreAttributeChange�������������Ըı�ǰ�Ĵ���

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;	// ��дPostGameplayEffectExecute����������Ч��ִ�к�Ĵ���


	/** ����ֵ */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category="Vital Attributes")	// ��ͼֻ��������ʹ��OnRep_Health����������ΪVital Attributes����Ҫ���ԣ�
	FGameplayAttributeData Health;		// ����ֵ������ΪFGameplayAttributeData������һ���ṹ�壬�����˵�ǰֵ�ͻ���ֵ
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Vital Attributes")	// ��ͼֻ��������ΪVital Attributes����Ҫ���ԣ�
	FGameplayAttributeData MaxHealth;	// �������ֵ
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	/** ħ�� */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")	// ��ͼֻ��, ����ʹ��OnRep_Mana����������ΪVital Attributes����Ҫ���ԣ�
	FGameplayAttributeData Mana;		// ħ��
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Vital Attributes")	// ��ͼֻ��������ΪVital Attributes����Ҫ���ԣ�
	FGameplayAttributeData MaxMana;		// ���ħ��
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;		// һ����˵��Rep�����ǲ���Ҫ��������ģ������������Ҫ��Rep������ʹ�þ�ֵ����ô�����Ҫ�������

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;	// �������ֵ�ص�����

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;		// ħ���ص�����

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;	// ���ħ���ص�����
};
