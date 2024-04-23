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

	bReplicates = true;	// ��������

	// ����������ײ�壬������Ϊ����������ڼ����ײ��������������Ч����ֻ���ڼ�⣬�Զ�̬����;�̬���嶼�����ص����Խ�ɫ�����ص�
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// ����Ͷ�����ƶ������������Ϊ������ײ����ƶ����
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(Sphere);	// ���ø������
	ProjectileMovement->InitialSpeed = 1550.f;	// ��ʼ�ٶ�
	ProjectileMovement->MaxSpeed = 1550.f;	// ����ٶ�
	ProjectileMovement->ProjectileGravityScale = 0.f;	// Ͷ������������

}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	// ���û������������
	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());	// ����ѭ������,���󶨵������
	
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		// ֹͣ������Ч
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();

		// ��������
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);

		// ������Ч
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(1.f));

		bHit = true;
	}
	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectSpecHandle.Data.IsValid()) return;
	if (DamageEffectSpecHandle.Data.Get()->GetEffectContext().GetEffectCauser() == OtherActor)
	{
		// ������Լ�������Ͷ����Ͳ�����
		return;
	}

	// ������Ѿ���������
	if (UAuraAbilitySystemLibrary::IsFriendly(DamageEffectSpecHandle.Data.Get()->GetEffectContext().GetEffectCauser(), OtherActor)) return;

	if (!bHit)
	{
		// ֹͣ������Ч
		if (LoopingSoundComponent) LoopingSoundComponent->Stop();
		// ��������
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		// ������Ч
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(1.f));

		bHit = true;
	}

	if (HasAuthority())
	{
		// Ӧ���˺�Ч��
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}

		Destroy();
		// ��Ȼ������ǿͻ��ˣ�����ϣ���ͻ��˱�����֮ǰ���ҿ�������ĳ��bool������˵��������Ѿ������ˣ�
		// Ȼ��ͻ��˽��յ�������Ϊ�����ǿ��Լ�����bool�������ͻ����Ƿ��Ѿ����Ź���Ч�ˣ����û�У������ڿͻ��˲���һ��
	}
	else
	{
		bHit = true;
	}

	

}

