// Copyright INVI1998

#pragma once

#include "AbilitySystemInterface.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;

UCLASS(Abstract)	// ���Abstract�ؼ��֣���ʾ�������һ��������
class ARCANE_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;		// ����

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

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const;	// ���Լ�Ӧ��Ч��
	void InitializeDefaultAttributes() const;		// ��ʼ������

};
