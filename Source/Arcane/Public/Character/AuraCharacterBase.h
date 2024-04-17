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

UCLASS(Abstract)	// ���Abstract�ؼ��֣���ʾ�������һ��������
class ARCANE_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;	// ��ȡ�ܻ���Ӧ����

	virtual UAnimMontage* GetDeathMontage_Implementation() override;	// ��ȡ��������

	virtual void Die() override;	// ����

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();	// �ಥ��������

protected:
	virtual void BeginPlay() override;

	virtual void InitializeDefaultAttributes() const;		// ��ʼ������

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;		// ����

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> BowWeapon;		// ��������

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> BowArrow;			// ��

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;	// ������˲������

	virtual FVector GetCombatSocketLocation() const override;	// ��ȡս�����λ��

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;	// ����ϵͳ���

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;	// ���Լ�

	virtual void InitAbilityActorInfo();		// ��ʼ��������ɫ��Ϣ

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryGameplayEffectClass;		// Ĭ����Ҫ��ϷЧ����

	// �����һ�������Ե���ϷЧ��������ʼ�մ����ڽ�ɫ���ϣ�ÿ����ɫ�������Է����仯ʱ����Щ��Ҫ����Ҳ�ᷢ�����Ӧ�ĵ���
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryGameplayEffectClass;	// Ĭ�ϴ�Ҫ��ϷЧ����

	// ����һ��˲ʱ�Ե���ϷЧ���������ڽ�ɫ��ʼ��ʱ��Ӧ�ã�Ȼ��������ʧ
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalGameplayEffectClass;		// Ĭ����Ҫ��ϷЧ����

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;	// ���Լ�Ӧ��Ч��
	

	/*
	 * Ability
	 */
	void AddCharacterAbilities();	// ��ӽ�ɫ����


	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bIsHitReact = false;	// �Ƿ��ܻ�

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;	// ���������ٶ�

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;	// �ܻ���Ӧ����

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> DeathMontage;	// ��������

	/*
	 * Dissolve Effect �ܽ�Ч��
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;	// �ܽ����ʵ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;	// �����ܽ����ʵ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> BowWeaponDissolveMaterialInstance;	// ���������ܽ����ʵ��

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> BowArrowDissolveMaterialInstance;		// ���ܽ����ʵ��

	void Dissolve();	// �ܽ�

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);	// ��ʼ�ܽ�ʱ����

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);	// ��ʼ�ܽ�ʱ����

	UFUNCTION(BlueprintImplementableEvent)
	void StartBowDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);	// ��ʼ�ܽ�ʱ����

	UFUNCTION(BlueprintImplementableEvent)
	void StartArrowDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);	// ��ʼ�ܽ�ʱ����


private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UAuraGameplayAbility>> StartupAbilities;	// Ĭ������

};
