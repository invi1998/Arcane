#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UAbilitySystemComponent;
class UAuraAbilitySystemComponent;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;		// 世界上下文对象

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageEffectClass;		// 伤害效果

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> InstigatorASC;	// 施加者ASC

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC;		// 目标ASC

	UPROPERTY()
	float BaseDamage = 0.f;		// 基础伤害

	UPROPERTY()
	float AbilityLevel = 1.f;	// 技能等级

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag::EmptyTag;	// 伤害类型

	UPROPERTY()
	float DebuffChance = 0.f;	// Debuff几率

	UPROPERTY()
	float DebuffDamage = 0.f;	// Debuff伤害

	UPROPERTY()
	float DebuffFrequency = 0.f;	// Debuff频率

	UPROPERTY()
	float DebuffDuration = 0.f;	// Debuff持续时间


};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }

	void SetCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// 网络序列化。这里决定了这个结构体在网络中如何序列化
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;		// 是否格挡

	UPROPERTY()
	bool bIsCriticalHit = false;	// 是否暴击
	
};

// 如果我们需要自定义GameplayEffectContext, 那么我们需要在这里注册GameplayEffectContext的操作类型
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,	// Necessary so that FGameplayEffectContext can be serialized over the network（必要的，以便FGameplayEffectContext可以在网络上序列化）
		WithCopy = true,	// Necessary so that TSharedPtr<FHitResult> Data is copied around（必要的，以便在各个地方复制TSharedPtr<FHitResult>数据）
	};
};
