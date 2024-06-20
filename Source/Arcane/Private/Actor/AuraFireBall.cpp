// Copyright INVI1998


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();

	StartOutgoingTimeLine();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//// 遍历伤害效果参数
	//for (auto& Pair : DamageEffectParams)
	//{
	//	// 获取伤害效果参数
	//	FDamageEffectParams& Params = Pair.Value;

	//	if (!IsValid(Params.InstigatorASC))
	//	{
	//		// 如果没有施法者ASC，不处理 添加这一行。这将解决客户端火球问题。
	//		return;
	//	}

	//	AActor* SourceActor = Params.InstigatorASC->GetAvatarActor();

	//	// 如果没有施法者，不处理
	//	if (SourceActor == nullptr) return;

	//	if (OtherActor == SourceActor)
	//	{
	//		// 如果是自己，不处理
	//		return;
	//	}

	//	if (UAuraAbilitySystemLibrary::IsFriendly(SourceActor, OtherActor))
	//	{
	//		// 如果是友军，不处理
	//		return;
	//	}

	//	if (!bHit)
	//	{
	//		OnHit();
	//	}

	//	if (HasAuthority())
	//	{
	//		// 应用伤害效果
	//		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	//		{
	//			// 设置死亡冲量，投射物的前向量乘以死亡冲量大小
	//			Params.DeathImpulse = GetActorForwardVector() * Params.DeathImpulseMagnitude;

	//			const bool bIsKnockback = FMath::RandRange(0, 100) <= Params.KnockbackChance;
	//			if (bIsKnockback)
	//			{
	//				// 设置击退冲量，投射物的前向量乘以击退冲量大小

	//				// 旋转击退冲量方向, 我们让其做一点随机旋转，这样击退效果看起来更加自然
	//				FRotator KnockbackRotator = GetActorRotation();
	//				KnockbackRotator.Pitch = FMath::RandRange(0.f, 45.f);
	//				const FVector KnockbackImpulseDir = KnockbackRotator.Vector();
	//				Params.KnockbackImpulse = KnockbackImpulseDir * Params.KnockbackMagnitude;
	//			}

	//			// 设置Params的目标Actor
	//			Params.TargetASC = TargetASC;

	//			// 应用伤害效果
	//			UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);
	//		}
	//	}
	//	else
	//	{
	//		bHit = true;
	//	}
	//}

	//if (HasAuthority())
	//{
	//	// 销毁投射物
	//	Destroy();
	//}
}
