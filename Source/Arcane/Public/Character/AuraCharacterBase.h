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

UCLASS(Abstract)	// 添加Abstract关键字，表示这个类是一个抽象类
class ARCANE_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;


protected:
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const;		// 初始化能力

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;		// 武器

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;	// 武器尖端插槽名称

	virtual FVector GetCombatSocketLocation() const override;	// 获取战斗插槽位置

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

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;	// 基础行走速度

private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbilities;	// 默认能力

};
