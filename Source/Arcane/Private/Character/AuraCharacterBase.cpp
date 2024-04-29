// Copyright INVI1998

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Arcane/Arcane.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraSystem.h"

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
	Weapon->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	BowWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BowWeapon"));	// ���������������
	BowWeapon->SetupAttachment(GetMesh(), FName("SKT_Bow"));		// ������������ĸ������Mesh���͹��ص�
	BowWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// ���������������ײ״̬
	BowWeapon->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	BowArrow = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BowArrow"));	// ���������
	BowArrow->SetupAttachment(GetMesh(), FName("SKT_Arrow"));		// ������������ĸ������Mesh���͹��ص�
	BowArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// ���������������ײ״̬
	BowArrow->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	LeftWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("LeftWeapon"));	// ���������������
	LeftWeapon->SetupAttachment(GetMesh(), FName("LeftWeaponSocket"));		// ����������������ĸ������Mesh���͹��ص�
	LeftWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// �������������������ײ״̬
	LeftWeapon->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	RightWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("RightWeapon"));	// ���������������
	RightWeapon->SetupAttachment(GetMesh(), FName("RightWeaponSocket"));		// ����������������ĸ������Mesh���͹��ص�
	RightWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// �������������������ײ״̬
	RightWeapon->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

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

UAnimMontage* AAuraCharacterBase::GetDeathMontage_Implementation()
{
	return DeathMontage;
}

void AAuraCharacterBase::Die()
{
	//Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// �����������Ĺ���
	//BowWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// ���������������Ĺ���
	//BowArrow->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// ���������Ĺ���
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	//Weapon->SetSimulatePhysics(true);	// �����������ģ������
	//Weapon->SetEnableGravity(true);		// �������������������
	//Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// �������������ײ״̬,ֻ������

	//BowWeapon->SetSimulatePhysics(true);	// ���ù����������ģ������
	//BowWeapon->SetEnableGravity(true);		// ���ù������������������
	//BowWeapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// ���ù������������ײ״̬,ֻ������

	//BowArrow->SetSimulatePhysics(true);	// ���ü����ģ������
	//BowArrow->SetEnableGravity(true);		// ���ü������������
	//BowArrow->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// ���ü������ײ״̬,ֻ������

	//GetMesh()->SetEnableGravity(true);	// ����Mesh��������
	//GetMesh()->SetSimulatePhysics(true);	// ����Meshģ������
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// ����Mesh��ײ״̬,ֻ������

	//GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);	// ����Mesh��ײ��Ӧ

	bDead = true;	// ��������״̬

	Dissolve();	// �ܽ�

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// ���ý�������ײ״̬

}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		// ��ȡ������˲�۵�λ��
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftWeaponLeftSwing) && IsValid(LeftWeapon))
	{
		// ��ȡ����������۵�λ��
		return LeftWeapon->GetSocketLocation(LeftWeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftWeaponRightSwing) && IsValid(LeftWeapon))
	{
		// ��ȡ����������۵�λ��
		return LeftWeapon->GetSocketLocation(LeftWeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightWeaponLeftSwing) && IsValid(RightWeapon))
	{
		// ��ȡ����������۵�λ��
		return RightWeapon->GetSocketLocation(RightWeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightWeaponRightSwing) && IsValid(RightWeapon))
	{
		// ��ȡ����������۵�λ��
		return RightWeapon->GetSocketLocation(RightWeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftUnarmed))
	{
		// ��ȡ���ֿ��ֹ�����۵�λ��
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightUnarmed))
	{
		// ��ȡ���ֿ��ֹ�����۵�λ��
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_Bow))
	{
		// ��ȡ������˲�۵�λ��
		return BowArrow->GetSocketLocation(BowArrowTipSocketName);
	}

	return FVector::ZeroVector;
}

FVector AAuraCharacterBase::GetCombatSocketForward_Implementation(const FGameplayTag& MontageTag) const
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		// ��ȡ������˲�۵�ǰ������
		return Weapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftWeaponLeftSwing) && IsValid(LeftWeapon))
	{
		// ��ȡ����������۵�ǰ������
		return LeftWeapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftWeaponRightSwing) && IsValid(LeftWeapon))
	{
		// ��ȡ����������۵�ǰ������
		return LeftWeapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightWeaponLeftSwing) && IsValid(RightWeapon))
	{
		// ��ȡ����������۵�ǰ������
		return RightWeapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightWeaponRightSwing) && IsValid(RightWeapon))
	{
		// ��ȡ����������۵�ǰ������
		return RightWeapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftUnarmed))
	{
		// ��ȡ���ֿ��ֹ�����۵�ǰ������
		return GetMesh()->GetSocketTransform(LeftHandSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightUnarmed))
	{
		// ��ȡ���ֿ��ֹ�����۵�ǰ������
		return GetMesh()->GetSocketTransform(RightHandSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_Bow))
	{
		// ��ȡ������˲�۵�ǰ������
		return BowArrow->GetSocketTransform(BowArrowTipSocketName).GetRotation().GetForwardVector();
	}

	return FVector::ZeroVector;
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
	if (IsValid(DissolveMaterialInstance2))
	{
		// ������̬����ʵ��
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(DissolveMaterialInstance2, this);
		// ���ö�̬����ʵ���Ĳ���
		GetMesh()->SetMaterial(1, MID);
		StartDissolveTimeline(MID);
	}
	if (IsValid(Weapon) && IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, MID);
		StartWeaponDissolveTimeline(MID);
	}
	if (IsValid(LeftWeapon) && IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		LeftWeapon->SetMaterial(0, MID);
		StartWeaponDissolveTimeline(MID);
	}
	if (IsValid(RightWeapon) && IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		RightWeapon->SetMaterial(0, MID);
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

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetActor_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation() const
{
	return AttackMontages;
}

FTaggedMontage AAuraCharacterBase::GetRandomAttackMontage_Implementation(const FGameplayTag& AbilityTag) const
{
	if (AttackMontages.Num() == 0) return FTaggedMontage();

	// ��AttackMontages�����л�ȡAbilityTag��ǩ��Montage�����±�
	TArray<int32> MontageIndices;
	for (int32 i = 0; i < AttackMontages.Num(); i++)
	{
		if (AttackMontages[i].AbilityTag.MatchesTagExact(AbilityTag))
		{
			MontageIndices.Add(i);
		}
	}
	if (MontageIndices.Num() == 0) return FTaggedMontage();
	// ����ӹ������������л�ȡһ������
	const int32 RandomIndex = FMath::RandRange(0, MontageIndices.Num() - 1);
	return AttackMontages[MontageIndices[RandomIndex]];
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetMontageByTag_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& MontageTag) const
{
	for (const FTaggedMontage& TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag.MatchesTagExact(MontageTag) && TaggedMontage.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetSummonCount_Implementation()
{
	return NumMinions;
}

void AAuraCharacterBase::IncrementSummonCount_Implementation(int32 Amount)
{
	NumMinions += Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}


