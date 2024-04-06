// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

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

	/** 生命值 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_Health, Category="Vital Attributes")	// 蓝图只读，复制使用OnRep_Health函数，分类为Vital Attributes（重要属性）
	FGameplayAttributeData Health;		// 生命值，类型为FGameplayAttributeData，这是一个结构体，包含了当前值和基础值

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Vital Attributes")	// 蓝图只读，分类为Vital Attributes（重要属性）
	FGameplayAttributeData MaxHealth;	// 最大生命值

	/** 魔法 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Mana Attributes")	// 蓝图只读, 复制使用OnRep_Mana函数，分类为Mana Attributes（魔法属性）
	FGameplayAttributeData Mana;		// 魔法

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Mana Attributes")	// 蓝图只读，分类为Mana Attributes（魔法属性）
	FGameplayAttributeData MaxMana;		// 最大魔法

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;		// 一般来说，Rep函数是不需要传入参数的，但是如果你需要在Rep函数中使用旧值，那么你就需要传入参数

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;	// 最大生命值回调函数

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;		// 魔法回调函数

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;	// 最大魔法回调函数
};
