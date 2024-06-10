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

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;		// 世界上下文对象

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageEffectClass;		// 伤害效果

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> InstigatorASC;	// 施加者ASC

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetASC;		// 目标ASC

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;		// 基础伤害

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;	// 技能等级

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag::EmptyTag;	// 伤害类型

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;	// Debuff几率

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;	// Debuff伤害

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;	// Debuff频率

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;	// Debuff持续时间

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;	// 死亡冲量大小

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;	// 死亡冲量

	UPROPERTY(BlueprintReadWrite)
	float KnockbackMagnitude = 0.f;	// 击退大小

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;	// 击退几率

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackImpulse = FVector::ZeroVector;	// 击退冲量
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit() const { return bIsBlockedHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	float GetDebuffDuration() const { return DebuffDuration; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackImpulse() const { return KnockbackImpulse; }

	void SetCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetSuccessfulDebuff(bool bInIsSuccessfulDebuff) { bIsSuccessfulDebuff = bInIsSuccessfulDebuff; }
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage; }
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency; }
	void SetDebuffDuration(float InDebuffDuration) { DebuffDuration = InDebuffDuration; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackImpulse(const FVector& InKnockbackImpulse) { KnockbackImpulse = InKnockbackImpulse; }

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

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;	// 是否成功Debuff

	UPROPERTY()
	float DebuffDamage = 0.f;	// Debuff伤害

	UPROPERTY()
	float DebuffFrequency = 0.f;	// Debuff频率

	UPROPERTY()
	float DebuffDuration = 0.f;	// Debuff持续时间

	TSharedPtr<FGameplayTag> DamageType = nullptr;	// 伤害类型

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;	// 死亡冲量

	UPROPERTY()
	FVector KnockbackImpulse = FVector::ZeroVector;	// 击退冲量
	
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
