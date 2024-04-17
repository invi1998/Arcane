// Copyright INVI1998

#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Arcane/Arcane.h"
#include "Components/CapsuleComponent.h"

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

	BowWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BowWeapon"));	// 创建弓箭武器组件
	BowWeapon->SetupAttachment(GetMesh(), FName("SKT_Bow"));		// 设置武器组件的父组件（Mesh）和挂载点
	BowWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置武器组件的碰撞状态

	BowArrow = CreateDefaultSubobject<USkeletalMeshComponent>(FName("BowArrow"));	// 创建箭组件
	BowArrow->SetupAttachment(GetMesh(), FName("SKT_Arrow"));		// 设置武器组件的父组件（Mesh）和挂载点
	BowArrow->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// 设置武器组件的碰撞状态

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
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// 解除武器组件的挂载
	BowWeapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// 解除弓箭武器组件的挂载
	BowArrow->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));	// 解除箭组件的挂载
	MulticastHandleDeath();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	Weapon->SetSimulatePhysics(true);	// 设置武器组件模拟物理
	Weapon->SetEnableGravity(true);		// 设置武器组件启用重力
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// 设置武器组件碰撞状态,只有物理

	BowWeapon->SetSimulatePhysics(true);	// 设置弓箭武器组件模拟物理
	BowWeapon->SetEnableGravity(true);		// 设置弓箭武器组件启用重力
	BowWeapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// 设置弓箭武器组件碰撞状态,只有物理

	BowArrow->SetSimulatePhysics(true);	// 设置箭组件模拟物理
	BowArrow->SetEnableGravity(true);		// 设置箭组件启用重力
	BowArrow->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// 设置箭组件碰撞状态,只有物理

	GetMesh()->SetEnableGravity(true);	// 设置Mesh启用重力
	GetMesh()->SetSimulatePhysics(true);	// 设置Mesh模拟物理
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);	// 设置Mesh碰撞状态,只有物理

	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);	// 设置Mesh碰撞响应

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 设置胶囊体碰撞状态

	Dissolve();	// 溶解

}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector AAuraCharacterBase::GetCombatSocketLocation() const
{
	checkf(Weapon, TEXT("Weapon is nullptr!"));	// 检查武器是否为空
	// 获取武器尖端插槽的位置
	return Weapon->GetSocketLocation(WeaponTipSocketName);
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


