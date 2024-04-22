// Copyright INVI1998

#pragma once

#include "AbilitySystemInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UAuraGameplayAbility;
class UAnimMontage;

UCLASS(Abstract)	// 添加Abstract关键字，表示这个类是一个抽象类
class ARCANE_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();	// 多播处理死亡

	/*
	* Combat Interface begin
	*/
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;	// 获取战斗插槽位置
	virtual FVector GetCombatSocketForward_Implementation(const FGameplayTag& MontageTag) const override;	// 获取战斗插槽前向向量
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;	// 获取受击反应动画
	virtual UAnimMontage* GetDeathMontage_Implementation() override;	// 获取死亡动画
	virtual void Die() override;	// 死亡
	virtual bool IsDead_Implementation() const override;	// 是否死亡
	virtual AActor* GetActor_Implementation() override;	// 获取角色
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override;	// 获取攻击动画
	virtual FTaggedMontage GetRandomAttackMontage_Implementation() const override;
	/* Combat Interface End*/

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;	// 攻击动画

protected:
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const;		// 初始化能力

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;		// 武器

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> LeftWeapon;		// 左手武器

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> RightWeapon;		// 右手武器

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> BowWeapon;		// 弓箭武器

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> BowArrow;			// 箭

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName WeaponTipSocketName;	// 武器尖端插槽名称

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName BowArrowTipSocketName;		// 弓箭尖端插槽名称

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName LeftHandSocketName;	// 左手插槽名称

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName RightHandSocketName;	// 右手插槽名称

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName LeftWeaponTipSocketName;	// 左手武器尖端插槽名称

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	FName RightWeaponTipSocketName;	// 右手武器尖端插槽名称

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;	// 能力系统组件

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;	// 属性集

	virtual void InitAbilityActorInfo();		// 初始化能力角色信息

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryGameplayEffectClass;		// 默认主要游戏效果类

	// 这会是一个永久性的游戏效果，它将始终存在于角色身上，每当角色的主属性发生变化时，这些次要属性也会发生相对应的调整
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryGameplayEffectClass;	// 默认次要游戏效果类

	// 这是一个瞬时性的游戏效果，它会在角色初始化时被应用，然后立即消失
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalGameplayEffectClass;		// 默认重要游戏效果类

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;	// 对自己应用效果
	

	/*
	 * Ability
	 */
	void AddCharacterAbilities();	// 添加角色能力


	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsHitReact = false;	// 是否受击

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;	// 基础行走速度

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;	// 受击反应动画

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> DeathMontage;	// 死亡动画

	/*
	 * Dissolve Effect 溶解效果
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;	// 溶解材质实例

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;	// 武器溶解材质实例

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> BowWeaponDissolveMaterialInstance;	// 弓箭武器溶解材质实例

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> BowArrowDissolveMaterialInstance;		// 箭溶解材质实例

	void Dissolve();	// 溶解

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);	// 开始溶解时间轴

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);	// 开始溶解时间轴

	UFUNCTION(BlueprintImplementableEvent)
	void StartBowDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);	// 开始溶解时间轴

	UFUNCTION(BlueprintImplementableEvent)
	void StartArrowDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);	// 开始溶解时间轴

	bool bDead = false;	// 是否死亡

private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbilities;	// 默认能力

};
