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

// typedef is specific to the FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;


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

	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributes;	// GamplayTag和一个静态函数指针的映射，用于将GamplayTag映射到属性，或者后续用于绑定其他的回调函数

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

	/** 魔法 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")	// 蓝图只读, 复制使用OnRep_Mana函数，分类为Vital Attributes（重要属性）
	FGameplayAttributeData Mana;		// 魔法
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;		// 一般来说，Rep函数是不需要传入参数的，但是如果你需要在Rep函数中使用旧值，那么你就需要传入参数

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;		// 魔法回调函数


	/*
	 * 次要属性（Secondary Attributes） 或者说是派生属性（Derived Attributes）
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData MaxHealth;	// 最大生命值
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData MaxMana;		// 最大魔法
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数


	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category="Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData Armor;		// 护甲
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category="Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData ArmorPenetration;		// 护甲穿透
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category="Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData BlockChance;		// 格挡几率
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category="Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData CriticalHitChance;		// 暴击几率
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category="Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData CriticalHitDamage;		// 暴击伤害
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category="Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData CriticalHitResistance;		// 暴击抗性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category="Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData HealthRegeneration;		// 生命恢复
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category="Secondary Attributes")	// 蓝图只读，分类为Secondary Attributes（次要属性）
	FGameplayAttributeData ManaRegeneration;		// 魔法恢复
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, Category="Secondary Attributes|Resistance Attributes", ReplicatedUsing = OnRep_FireResistance)	// 蓝图只读，分类为Secondary Attributes（次要属性）和Resistance Attributes（抗性属性）
	FGameplayAttributeData FireResistance;		// 火焰抗性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, Category="Secondary Attributes|Resistance Attributes", ReplicatedUsing = OnRep_IceResistance)	// 蓝图只读，分类为Secondary Attributes（次要属性）和Resistance Attributes（抗性属性）
	FGameplayAttributeData IceResistance;		// 冰霜抗性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IceResistance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, Category="Secondary Attributes|Resistance Attributes", ReplicatedUsing = OnRep_LightningResistance)	// 蓝图只读，分类为Secondary Attributes（次要属性）和Resistance Attributes（抗性属性）
	FGameplayAttributeData LightningResistance;		// 闪电抗性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, Category="Secondary Attributes|Resistance Attributes", ReplicatedUsing = OnRep_PoisonResistance)	// 蓝图只读，分类为Secondary Attributes（次要属性）和Resistance Attributes（抗性属性）
	FGameplayAttributeData PoisonResistance;		// 毒素抗性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PoisonResistance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, Category="Secondary Attributes|Resistance Attributes", ReplicatedUsing = OnRep_ArcaneResistance)	// 蓝图只读，分类为Secondary Attributes（次要属性）和Resistance Attributes（抗性属性）
	FGameplayAttributeData ArcaneResistance;		// 奥术抗性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, Category="Secondary Attributes|Resistance Attributes", ReplicatedUsing = OnRep_PhysicalResistance)	// 蓝图只读，分类为Secondary Attributes（次要属性）和Resistance Attributes（抗性属性）
	FGameplayAttributeData PhysicalResistance;		// 物理抗性
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;	// 最大生命值回调函数

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;	// 最大魔法回调函数

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;		// 护甲回调函数

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const;	// 护甲穿透回调函数

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;	// 格挡几率回调函数

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const;	// 暴击几率回调函数

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const;	// 暴击伤害回调函数

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const;	// 暴击抗性回调函数

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const;	// 生命恢复回调函数

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const;	// 魔法恢复回调函数

	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;	// 火焰抗性回调函数

	UFUNCTION()
	void OnRep_IceResistance(const FGameplayAttributeData& OldIceResistance) const;	// 冰霜抗性回调函数

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const;	// 闪电抗性回调函数

	UFUNCTION()
	void OnRep_PoisonResistance(const FGameplayAttributeData& OldPoisonResistance) const;	// 毒素抗性回调函数

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;	// 奥术抗性回调函数

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;	// 物理抗性回调函数

	/*
	 * Meta Attributes 元属性, 用于描述角色的特性
	 */
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")	// 蓝图只读，分类为Meta Attributes（元属性）, 元属性不需要复制
	FGameplayAttributeData IncomingDamage;	// 元属性：受到的伤害
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数

	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")	// 蓝图只读，分类为Meta Attributes（元属性）, 元属性不需要复制
	FGameplayAttributeData RewardExperience;	// 元属性：奖励经验
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, RewardExperience)	// 生成属性的Getter函数，属性的Setter函数，属性的初始化函数


private:
	void SetEffectsProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties) const;	// 设置效果属性

	void ShowFloatingText(const FEffectProperties Props, float Damage, bool bBlockedHit, bool bCriticalHit) const;	// 显示浮动文字，用于显示伤害，格挡，暴击等信息

	void SendEXPEvent(const FEffectProperties& Props) const;	// 发送经验给玩家
};
