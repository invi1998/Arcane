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


USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
		: SourceASC(nullptr)
		, SourceAvatarActor(nullptr)
		, SourceController(nullptr)
		, SourceCharacter(nullptr)
		, TargetASC(nullptr)
		, TargetAvatarActor(nullptr)
		, TargetController(nullptr)
		, TargetCharacter(nullptr)
	{}

	/*
	 * Source
	 */
	UPROPERTY()
	UAbilitySystemComponent* SourceASC;		// ʩ���ߵ�����ϵͳ���

	UPROPERTY()
	AActor* SourceAvatarActor;					// ʩ���ߵ�Actor

	UPROPERTY()
	AController* SourceController;			// ʩ���ߵĿ�����

	UPROPERTY()
	ACharacter* SourceCharacter;			// ʩ���ߵĽ�ɫ

	/*
	 * Target
	 */

	UPROPERTY()
	UAbilitySystemComponent* TargetASC;		// Ŀ�������ϵͳ���

	UPROPERTY()
	AActor* TargetAvatarActor;					// Ŀ���Actor

	UPROPERTY()
	AController* TargetController;			// Ŀ��Ŀ�����

	UPROPERTY()
	ACharacter* TargetCharacter;			// Ŀ��Ľ�ɫ

	/*
	 * Context
	 */

	FGameplayEffectContextHandle EffectContextHandle;	// Ч�������ľ��
};



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


	/*
	 * RPG �����ԣ�Primary Attributes��
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;	// ����
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Primary Attributes")
	FGameplayAttributeData Agility;		// ����
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Agility)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;	// ����
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;		// ����
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;	// ����
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;	// �����ص�����

	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility) const;	// ���ݻص�����

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;	// �����ص�����

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;	// �����ص�����

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;	// ���Իص�����


	/*
	 * ��Ҫ���ԣ�Vital Attributes��
	 */

	/** ����ֵ */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category="Vital Attributes")	// ��ͼֻ��������ʹ��OnRep_Health����������ΪVital Attributes����Ҫ���ԣ�
	FGameplayAttributeData Health;		// ����ֵ������ΪFGameplayAttributeData������һ���ṹ�壬�����˵�ǰֵ�ͻ���ֵ
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	/** ħ�� */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")	// ��ͼֻ��, ����ʹ��OnRep_Mana����������ΪVital Attributes����Ҫ���ԣ�
	FGameplayAttributeData Mana;		// ħ��
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;		// һ����˵��Rep�����ǲ���Ҫ��������ģ������������Ҫ��Rep������ʹ�þ�ֵ����ô�����Ҫ�������

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;		// ħ���ص�����


	/*
	 * ��Ҫ���ԣ�Secondary Attributes�� ����˵���������ԣ�Derived Attributes��
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData MaxHealth;	// �������ֵ
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData MaxMana;		// ���ħ��
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category="Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData Armor;		// ����
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category="Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData ArmorPenetration;		// ���״�͸
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category="Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData BlockChance;		// �񵲼���
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category="Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData CriticalHitChance;		// ��������
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category="Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData CriticalHitDamage;		// �����˺�
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category="Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData CriticalHitResistance;		// ��������
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category="Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData HealthRegeneration;		// �����ָ�
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category="Secondary Attributes")	// ��ͼֻ��������ΪSecondary Attributes����Ҫ���ԣ�
	FGameplayAttributeData ManaRegeneration;		// ħ���ָ�
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration)	// �������Ե�Getter���������Ե�Setter���������Եĳ�ʼ������

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;	// �������ֵ�ص�����

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;	// ���ħ���ص�����

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;		// ���׻ص�����

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;	// ���״�͸�ص�����

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;	// �񵲼��ʻص�����

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;	// �������ʻص�����

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;	// �����˺��ص�����

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;	// �������Իص�����

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;	// �����ָ��ص�����

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;	// ħ���ָ��ص�����


private:
	void SetEffectsProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const;	// ����Ч������
};
