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


