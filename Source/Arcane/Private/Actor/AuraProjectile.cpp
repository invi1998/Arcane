// Copyright INVI1998


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Arcane/Arcane.h"
#include "Components/AudioComponent.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;	// 开启复制

	// 创建球形碰撞体，并设置为根组件，用于检测碰撞，但不产生物理效果，只用于检测，对动态物体和静态物体都产生重叠，对角色产生重叠
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 创建投射物移动组件，并设置为球形碰撞体的移动组件
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Sphere);	// 设置更新组件
	ProjectileMovement->InitialSpeed = 1550.f;	// 初始速度
	ProjectileMovement->MaxSpeed = 1550.f;	// 最大速度
	ProjectileMovement->ProjectileGravityScale = 0.f;	// 投射物重力缩放

}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 设置火球的生命周期
	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());	// 播放循环声音,并绑定到根组件
	
}

void AAuraProjectile::OnHit()
{
	// 停止飞行音效
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	// 播放声音
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);

	// 播放特效
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(1.f));

	bHit = true;
}

void AAuraProjectile::Destroyed()
{
	// 停止飞行音效
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	if (!bHit && !HasAuthority())
	{
		OnHit();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 遍历伤害效果参数
	for (auto& Pair : DamageEffectParams)
	{
		// 获取伤害效果参数
		FDamageEffectParams& Params = Pair.Value;

		if (!IsValid(Params.InstigatorASC))
		{
			// 如果没有施法者ASC，不处理 添加这一行。这将解决客户端火球问题。
			return;
		}

		AActor* SourceActor = Params.InstigatorASC->GetAvatarActor();

		// 如果没有施法者，不处理
		if (SourceActor == nullptr) return;

		if (OtherActor == SourceActor)
		{
			// 如果是自己，不处理
			return;
		}

		if (UAuraAbilitySystemLibrary::IsFriendly(SourceActor, OtherActor))
		{
			// 如果是友军，不处理
			return;
		}

		if (!bHit)
		{
			OnHit();
		}

		if (HasAuthority())
		{
			// 应用伤害效果
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				// 设置死亡冲量，投射物的前向量乘以死亡冲量大小
				Params.DeathImpulse = GetActorForwardVector() * Params.DeathImpulseMagnitude;

				const bool bIsKnockback = FMath::RandRange(0, 100) <= Params.KnockbackChance;
				if (bIsKnockback)
				{
					// 设置击退冲量，投射物的前向量乘以击退冲量大小

					// 旋转击退冲量方向, 我们让其做一点随机旋转，这样击退效果看起来更加自然
					FRotator KnockbackRotator = GetActorRotation();
					KnockbackRotator.Pitch = FMath::RandRange(0.f, 45.f);
					const FVector KnockbackImpulseDir = KnockbackRotator.Vector();
					Params.KnockbackImpulse = KnockbackImpulseDir * Params.KnockbackMagnitude;
				}

				// 设置Params的目标Actor
				Params.TargetASC = TargetASC;

				// 应用伤害效果
				UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);
			}

			Destroy();
			// 当然，如果是客户端，我们希望客户端被销毁之前，我可以设置某个bool变量，说，服务端已经销毁了，
			// 然后客户端接收到销毁行为，我们可以检测这个bool，看看客户端是否已经播放过特效了，如果没有，我们在客户端播放一次
		}
		else
		{
			bHit = true;
		}

	}

}

