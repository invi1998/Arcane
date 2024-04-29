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

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);	// 设置胶囊体碰撞响应
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);		// 只让Capsule和Mesh网格中的一个生成重叠事件，避免重叠事件重复，照成类似以二次伤害的问题

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);	// 设置Mesh碰撞响应
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);	// 设置Mesh生成重叠事件

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Weapon"));	// 创建武器组件
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));		// 设置武器组件的父组件（Mesh）和挂载点
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置武器组件的碰撞状态
	Weapon->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	BowWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BowWeapon"));	// 创建弓箭武器组件
	BowWeapon->SetupAttachment(GetMesh(), FName("SKT_Bow"));		// 设置武器组件的父组件（Mesh）和挂载点
	BowWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置武器组件的碰撞状态
	BowWeapon->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	BowArrow = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BowArrow"));	// 创建箭组件
	BowArrow->SetupAttachment(GetMesh(), FName("SKT_Arrow"));		// 设置武器组件的父组件（Mesh）和挂载点
	BowArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置武器组件的碰撞状态
	BowArrow->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	LeftWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("LeftWeapon"));	// 创建左手武器组件
	LeftWeapon->SetupAttachment(GetMesh(), FName("LeftWeaponSocket"));		// 设置左手武器组件的父组件（Mesh）和挂载点
	LeftWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置左手武器组件的碰撞状态
	LeftWeapon->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	RightWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("RightWeapon"));	// 创建右手武器组件
	RightWeapon->SetupAttachment(GetMesh(), FName("RightWeaponSocket"));		// 设置右手武器组件的父组件（Mesh）和挂载点
	RightWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置右手武器组件的碰撞状态
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
	//Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// 解除武器组件的挂载
	//BowWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// 解除弓箭武器组件的挂载
	//BowArrow->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// 解除箭组件的挂载
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	//Weapon->SetSimulatePhysics(true);	// 设置武器组件模拟物理
	//Weapon->SetEnableGravity(true);		// 设置武器组件启用重力
	//Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// 设置武器组件碰撞状态,只有物理

	//BowWeapon->SetSimulatePhysics(true);	// 设置弓箭武器组件模拟物理
	//BowWeapon->SetEnableGravity(true);		// 设置弓箭武器组件启用重力
	//BowWeapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// 设置弓箭武器组件碰撞状态,只有物理

	//BowArrow->SetSimulatePhysics(true);	// 设置箭组件模拟物理
	//BowArrow->SetEnableGravity(true);		// 设置箭组件启用重力
	//BowArrow->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// 设置箭组件碰撞状态,只有物理

	//GetMesh()->SetEnableGravity(true);	// 设置Mesh启用重力
	//GetMesh()->SetSimulatePhysics(true);	// 设置Mesh模拟物理
	//GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// 设置Mesh碰撞状态,只有物理

	//GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);	// 设置Mesh碰撞响应

	bDead = true;	// 设置死亡状态

	Dissolve();	// 溶解

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 设置胶囊体碰撞状态

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
		// 获取武器尖端插槽的位置
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftWeaponLeftSwing) && IsValid(LeftWeapon))
	{
		// 获取左手武器插槽的位置
		return LeftWeapon->GetSocketLocation(LeftWeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftWeaponRightSwing) && IsValid(LeftWeapon))
	{
		// 获取左手武器插槽的位置
		return LeftWeapon->GetSocketLocation(LeftWeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightWeaponLeftSwing) && IsValid(RightWeapon))
	{
		// 获取右手武器插槽的位置
		return RightWeapon->GetSocketLocation(RightWeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightWeaponRightSwing) && IsValid(RightWeapon))
	{
		// 获取右手武器插槽的位置
		return RightWeapon->GetSocketLocation(RightWeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftUnarmed))
	{
		// 获取左手空手攻击插槽的位置
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightUnarmed))
	{
		// 获取右手空手攻击插槽的位置
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_Bow))
	{
		// 获取弓箭尖端插槽的位置
		return BowArrow->GetSocketLocation(BowArrowTipSocketName);
	}

	return FVector::ZeroVector;
}

FVector AAuraCharacterBase::GetCombatSocketForward_Implementation(const FGameplayTag& MontageTag) const
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		// 获取武器尖端插槽的前向向量
		return Weapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftWeaponLeftSwing) && IsValid(LeftWeapon))
	{
		// 获取左手武器插槽的前向向量
		return LeftWeapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftWeaponRightSwing) && IsValid(LeftWeapon))
	{
		// 获取左手武器插槽的前向向量
		return LeftWeapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightWeaponLeftSwing) && IsValid(RightWeapon))
	{
		// 获取右手武器插槽的前向向量
		return RightWeapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightWeaponRightSwing) && IsValid(RightWeapon))
	{
		// 获取右手武器插槽的前向向量
		return RightWeapon->GetSocketTransform(WeaponTipSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_LeftUnarmed))
	{
		// 获取左手空手攻击插槽的前向向量
		return GetMesh()->GetSocketTransform(LeftHandSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_RightUnarmed))
	{
		// 获取右手空手攻击插槽的前向向量
		return GetMesh()->GetSocketTransform(RightHandSocketName).GetRotation().GetForwardVector();
	}
	if (MontageTag.MatchesTagExact(AuraTags.Montage_Attack_Bow))
	{
		// 获取弓箭尖端插槽的前向向量
		return BowArrow->GetSocketTransform(BowArrowTipSocketName).GetRotation().GetForwardVector();
	}

	return FVector::ZeroVector;
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectClass, float Level) const
{
	checkf(IsValid(GetAbilitySystemComponent()), TEXT("AbilitySystemComponent is nullptr!"));	// 检查AbilitySystemComponent是否为空
	checkf(EffectClass, TEXT("EffectClass is nullptr!"));	// 检查EffectClass是否为空

	// 1：获取上下文
	FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
	// 2：设置SourceObject
	EffectContext.AddSourceObject(this);
	// 3：创建效果
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(EffectClass, Level, EffectContext);
	// 4：应用效果到目标
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

	// 给角色添加技能，这一行为应该只能在服务端进行
	if (!HasAuthority() || !AuraASC) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);

}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		// 创建动态材质实例
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		// 设置动态材质实例的参数
		GetMesh()->SetMaterial(0, MID);
		StartDissolveTimeline(MID);
	}
	if (IsValid(DissolveMaterialInstance2))
	{
		// 创建动态材质实例
		UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(DissolveMaterialInstance2, this);
		// 设置动态材质实例的参数
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

	// 从AttackMontages数组中获取AbilityTag标签的Montage数组下标
	TArray<int32> MontageIndices;
	for (int32 i = 0; i < AttackMontages.Num(); i++)
	{
		if (AttackMontages[i].AbilityTag.MatchesTagExact(AbilityTag))
		{
			MontageIndices.Add(i);
		}
	}
	if (MontageIndices.Num() == 0) return FTaggedMontage();
	// 随机从攻击动画数组中获取一个动画
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


