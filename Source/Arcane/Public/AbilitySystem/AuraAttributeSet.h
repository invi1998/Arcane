// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"


// 该宏用于生成属性的Getter函数，属性的Setter函数，属性的初始化函数
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
	UAbilitySystemComponent* SourceASC;		// 施法者的能力系统组件

	UPROPERTY()
	AActor* SourceAvatarActor;					// 施法者的Actor

	UPROPERTY()
	AController* SourceController;			// 施法者的控制器

	UPROPERTY()
	ACharacter* SourceCharacter;			// 施法者的角色

	/*
	 * Target
	 */

	UPROPERTY()
	UAbilitySystemComponent* TargetASC;		// 目标的能力系统组件

	UPROPERTY()
	AActor* TargetAvatarActor;					// 目标的Actor

	UPROPERTY()
	AController* TargetController;			// 目标的控制器

	UPROPERTY()
	ACharacter* TargetCharacter;			// 目标的角色

	/*
	 * Context
	 */

	FGameplayEffectContextHandle EffectContextHandle;	// 效果上下文句柄
};



/**
 * 属性集
 */
UCLASS()
class ARCANE_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	// 重写GetLifetimeReplicatedProps函数，用于设置属性的复制方式

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;	// 重写PreAttributeChange函数，用于属性改变前的处理

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;	// 重写PostGameplayEffectExecute函数，用于效果执行后的处理


	/*
	 * RPG 主属性（Primary Attributes）
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Primary Attributes")
	FGameplayAttributeData Strength;	// 力量
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Primary Attributes")
	FGameplayAttributeData Agility;		// 敏捷
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Agility)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category = "Primary Attributes")
	FGameplayAttributeData Intelligence;	// 智力
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Primary Attributes")
	FGameplayAttributeData Vigor;		// 活力
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category = "Primary Attributes")
	FGameplayAttributeData Resilience;	// 韧性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;	// 力量回调函数

	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility) const;	// 敏捷回调函数

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;	// 智力回调函数

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;	// 活力回调函数

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;	// 韧性回调函数


	/*
	 * 重要属性（Vital Attributes）
	 */

	/** 生命值 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category="Vital Attributes")	// 蓝图只读，复制使用OnRep_Health函数，分类为Vital Attributes（重要属性）
	FGameplayAttributeData Health;		// 生命值，类型为FGameplayAttributeData，这是一个结构体，包含了当前值和基础值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Vital Attributes")	// 蓝图只读，分类为Vital Attributes（重要属性）
	FGameplayAttributeData MaxHealth;	// 最大生命值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	/** 魔法 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")	// 蓝图只读, 复制使用OnRep_Mana函数，分类为Vital Attributes（重要属性）
	FGameplayAttributeData Mana;		// 魔法
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Vital Attributes")	// 蓝图只读，分类为Vital Attributes（重要属性）
	FGameplayAttributeData MaxMana;		// 最大魔法
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;		// 一般来说，Rep函数是不需要传入参数的，但是如果你需要在Rep函数中使用旧值，那么你就需要传入参数

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;	// 最大生命值回调函数

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;		// 魔法回调函数

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;	// 最大魔法回调函数

private:
	void SetEffectsProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const;	// 设置效果属性
};
