// Copyright INVI1998

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Weapon"));	// �����������
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));		// ������������ĸ������Mesh���͹��ص�
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// ���������������ײ״̬
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::InitializePrimaryAbilities() const
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("AbilitySystemComponent is nullptr!"));	// ���AbilitySystemComponent�Ƿ�Ϊ��
	checkf(DefaultPrimaryGameplayEffectClass, TEXT("DefaultPrimaryGameplayEffectClass is nullptr!"));	// ���DefaultPrimaryGameplayEffectClass�Ƿ�Ϊ��

	// 1����ȡ������
	const FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	// 2������Ч��
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryGameplayEffectClass, 1.f, EffectContext);
	// 3��Ӧ��Ч����Ŀ��
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}


