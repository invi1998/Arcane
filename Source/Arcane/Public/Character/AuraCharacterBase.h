// Copyright INVI1998

#pragma once

#include "AbilitySystemInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UAuraGameplayAbility;
class UAnimMontage;
class UDebuffNiagaraComponent;
class UPassiveNiagaraComponent;

UCLASS(Abstract)	// 添加Abstract关键字，表示这个类是一个抽象类
class ARCANE_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;	// 获取生命周期复制属性
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;	// 获取能力系统组件
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;	// 承受伤害

	UAttributeSet* GetAttributeSet() const;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);	// 多播处理死亡

	/*
	* Combat Interface begin
	*/
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;	// 获取战斗插槽位置
	virtual FVector GetCombatSocketForward_Implementation(const FGameplayTag& MontageTag) const override;	// 获取战斗插槽前向向量
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;	// 获取受击反应动画
	virtual UAnimMontage* GetDeathMontage_Implementation() override;	// 获取死亡动画
	virtual void Die(const FVector& DeathImpulse) override;	// 死亡
	virtual bool IsDead_Implementation() const override;	// 是否死亡
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;		// 获取ASC注册委托
	virtual FOnCharacterDeath& GetOnCharacterDeathDelegate() override;	// 获取角色死亡委托
	virtual FOnDamageSignature& GetOnDamageDelegate() override;	// 获取伤害委托
	virtual AActor* GetActor_Implementation() override;	// 获取角色
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override;	// 获取攻击动画
	virtual FTaggedMontage GetRandomAttackMontage_Implementation(const FGameplayTag& AbilityTag) const override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;	// 获取血液特效
	virtual FTaggedMontage GetMontageByTag_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& MontageTag) const override;	// 根据Tag获取MontageTag
	virtual int32 GetSummonCount_Implementation() override;	// 获取召唤物数量
	virtual void IncrementSummonCount_Implementation(int32 Amount) override;	// 设置召唤物数量
	virtual ECharacterClass GetCharacterClass_Implementation() const override;	// 获取角色类别
	virtual USkeletalMeshComponent* GetWeaponMesh_Implementation() const override;	// 获取武器网格
	virtual USkeletalMeshComponent* GetCharacterMesh_Implementation() const override;	// 获取角色网格
	virtual void SetIsBeingShock_Implementation(bool bShock) override;	// 设置是否被电击
	virtual bool IsBeingShock_Implementation() const override;	// 是否被电击
	virtual void UpdateWeaponSocket_Implementation(bool bIsBoStaff) override;	// 更新武器插槽
	/* Combat Interface End*/

	FOnASCRegistered OnASCRegistered;		// ASC注册委托
	FOnCharacterDeath OnCharacterDeath;		// 角色死亡委托
	FOnDamageSignature OnDamageDelegate;	// 伤害委托

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;	// 攻击动画
	
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);	// 眩晕标签改变

	UPROPERTY(ReplicatedUsing = OnRep_Stunned, BlueprintReadOnly, Category = "Combat")
	bool bIsStunned = false;	// 是否被眩晕

	UPROPERTY(ReplicatedUsing = OnRep_Burned, BlueprintReadOnly, Category = "Combat")
	bool bIsBurn = false;	// 是否被灼烧

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Combat")
	bool bIsBeingShocked = false;	// 是否被电击

	UFUNCTION()
	virtual void OnRep_Stunned();	// 眩晕改变

	UFUNCTION()
	virtual void OnRep_Burned();	// 灼烧改变

	void SetCharacterClass(ECharacterClass InCharacterClass);	// 设置角色职业

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
	float BaseWalkSpeed = 600.f;	// 基础行走速度

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
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance2;	// 溶解材质实例(有些角色有两个溶解材质)


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

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bDead = false;	// 是否死亡

	/*
	 * Niagara Effect
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	UNiagaraSystem* BloodEffect;	// 血液特效

	/*
	 * Debuff Niagara Effect 减益buff特效
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffEffect;	// 灼烧特效

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffEffect;	// 眩晕特效

	/*
	 * Passive Niagara Effect 被动特效
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HalaOfProtectionPassiveEffect;	// 保护之光被动特效

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonPassiveEffect;	// 生命吸取被动特效

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonPassiveEffect;	// 法力吸取被动特效

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachPointComponent;	// 特效附着点

	/*
	 * Minions 召唤物
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	int32 NumMinions = 0;	// 召唤的数量


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;	// 敌人职业

private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbilities;	// 默认能力


	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupPassiveAbilities;	// 默认被动能力
};
