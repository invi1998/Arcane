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
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);		// ֻ��Capsule��Mesh�����е�һ�������ص��¼��������ص��¼��ظ����ճ������Զ����˺�������

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);	// ����Mesh��ײ��Ӧ
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);	// ����Mesh�����ص��¼�

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Weapon"));	// �����������
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));		// ������������ĸ������Mesh���͹��ص�
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// ���������������ײ״̬

	BowWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BowWeapon"));	// ���������������
	BowWeapon->SetupAttachment(GetMesh(), FName("SKT_Bow"));		// ������������ĸ������Mesh���͹��ص�
	BowWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// ���������������ײ״̬

	BowArrow = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BowArrow"));	// ���������
	BowArrow->SetupAttachment(GetMesh(), FName("SKT_Arrow"));		// ������������ĸ������Mesh���͹��ص�
	BowArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// ���������������ײ״̬

}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// �����������Ĺ���
	BowWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// ���������������Ĺ���
	BowArrow->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// ���������Ĺ���
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);	// �����������ģ������
	Weapon->SetEnableGravity(true);		// �������������������
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// �������������ײ״̬,ֻ������

	BowWeapon->SetSimulatePhysics(true);	// ���ù����������ģ������
	BowWeapon->SetEnableGravity(true);		// ���ù������������������
	BowWeapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// ���ù������������ײ״̬,ֻ������

	BowArrow->SetSimulatePhysics(true);	// ���ü����ģ������
	BowArrow->SetEnableGravity(true);		// ���ü������������
	BowArrow->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// ���ü������ײ״̬,ֻ������

	GetMesh()->SetEnableGravity(true);	// ����Mesh��������
	GetMesh()->SetSimulatePhysics(true);	// ����Meshģ������
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// ����Mesh��ײ״̬,ֻ������

	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);	// ����Mesh��ײ��Ӧ

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// ���ý�������ײ״̬

	Dissolve();	// �ܽ�

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

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		// ������̬����ʵ��
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		// ���ö�̬����ʵ���Ĳ���
		GetMesh()->SetMaterial(0, MID);
		StartDissolveTimeline(MID);
	}
	if (IsValid(Weapon) && IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, MID);
		StartWeaponDissolveTimeline(MID);
	}
	if (IsValid(BowWeapon) && IsValid(BowWeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BowWeaponDissolveMaterialInstance, this);
		BowWeapon->SetMaterial(0, MID);
		StartBowDissolveTimeline(MID);
	}
	if (IsValid(BowArrow) && IsValid(BowArrowDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BowArrowDissolveMaterialInstance, this);
		BowArrow->SetMaterial(0, MID);
		StartArrowDissolveTimeline(MID);
	}
}


