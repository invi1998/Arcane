// Copyright INVI1998

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Arcane/Arcane.h"
#include "Components/CapsuleComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);	// ���ý�������ײ��Ӧ
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);	// ����Mesh��ײ��Ӧ
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);	// ����Mesh�����ص��¼�

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

FVector AAuraCharacterBase::GetCombatSocketLocation() const
{
	checkf(Weapon, TEXT("Weapon is nullptr!"));	// ��������Ƿ�Ϊ��
	// ��ȡ������˲�۵�λ��
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("AbilitySystemComponent is nullptr!"));	// ���AbilitySystemComponent�Ƿ�Ϊ��
	checkf(EffectClass, TEXT("EffectClass is nullptr!"));	// ���EffectClass�Ƿ�Ϊ��

	// 1����ȡ������
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	// 2������SourceObject
	EffectContext.AddSourceObject(this);
	// 3������Ч��
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, EffectContext);
	// 4��Ӧ��Ч����Ŀ��
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryGameplayEffectClass, 1.f);
	ApplyEffectToSelf(DefaultSecondaryGameplayEffectClass, 1.f);
	ApplyEffectToSelf(DefaultVitalGameplayEffectClass, 1.f);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());

	// ����ɫ��Ӽ��ܣ���һ��ΪӦ��ֻ���ڷ���˽���
	if (!HasAuthority() || !AuraASC) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);

}


