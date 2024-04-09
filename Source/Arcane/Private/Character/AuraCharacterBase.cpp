// Copyright INVI1998

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Weapon"));	// 创建武器组件
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));		// 设置武器组件的父组件（Mesh）和挂载点
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置武器组件的碰撞状态
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
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("AbilitySystemComponent is nullptr!"));	// 检查AbilitySystemComponent是否为空
	checkf(DefaultPrimaryGameplayEffectClass, TEXT("DefaultPrimaryGameplayEffectClass is nullptr!"));	// 检查DefaultPrimaryGameplayEffectClass是否为空

	// 1：获取上下文
	const FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	// 2：创建效果
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryGameplayEffectClass, 1.f, EffectContext);
	// 3：应用效果到目标
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}


