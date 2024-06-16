// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "CombatInterface.generated.h"

class UAnimMontage;
class UNiagaraSystem;
class UAbilitySystemComponent;

// 该委托用于注册ASC，当ASC被创建时，调用该委托
DECLARE_MULTICAST_DELEGATE_OneParam(FOnASCRegistered, UAbilitySystemComponent*);
// 角色死亡委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeath, AActor*, DeadActor);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundBase* ImpactSound;	// 攻击声音

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNiagaraSystem* ImpactEffect;	// 攻击特效
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)	// 将接口类型设置为蓝图类型
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANE_API ICombatInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const;	// 获取战斗插槽位置

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	FVector GetCombatSocketForward(const FGameplayTag& MontageTag) const;	// 获取战斗插槽前向向量

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)		// 蓝图实现，同时蓝图可调用
	void UpdateFacingTarget(const FVector& Target);	//更新面向目标

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// 蓝图原生事件，蓝图可以重写
	UAnimMontage* GetHitReactMontage();	// 获取受击反应动画

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)	// 蓝图原生事件，蓝图可以重写
	UAnimMontage* GetDeathMontage();	// 获取死亡动画

	virtual void Die(const FVector& DeathImpulse) = 0;	// 死亡

	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() = 0;	// 获取ASC注册委托

	virtual FOnCharacterDeath& GetOnCharacterDeathDelegate() = 0;	// 获取角色死亡委托

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	bool IsDead() const;	// 是否死亡

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	AActor* GetActor();	// 获取Actor

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	TArray<FTaggedMontage> GetAttackMontages() const;	// 获取攻击动画

	// 从TArray中随机选择一个Montage
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	FTaggedMontage GetRandomAttackMontage(const FGameplayTag& AbilityTag) const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	UNiagaraSystem* GetBloodEffect();

	// 根据Tag获取MontageTag
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	FTaggedMontage GetMontageByTag(const FGameplayTag& AbilityTag, const FGameplayTag& MontageTag) const;

	// 获取召唤物数量
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	int32 GetSummonCount();

	// 设置召唤物数量
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	void IncrementSummonCount(int32 Amount);

	// 获取投射物类
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	TSubclassOf<AAuraProjectile> GetProjectileClass();

	// 获取角色职业
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)	// 蓝图可调用，蓝图原生事件
	ECharacterClass GetCharacterClass() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetInShockLoop(bool bInLoop);		// 设置是否处于震荡循环中

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() const;	// 获取武器Mesh

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsBeingShock() const;	// 是否处于震荡循环中

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetIsBeingShock(bool bShock);	// 设置是否处于震荡循环中

};
