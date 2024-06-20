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
	// 遍历伤害效果参数
	for (auto& Pair : DamageEffectParams)
	{
		// 获取伤害效果参数
		FDamageEffectParams& Params = Pair.Value;

		if (IsNotValidOverlap(OtherActor, Params)) return;

		if (HasAuthority())
		{
			// 应用伤害效果
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				// 设置Params的目标Actor
				Params.TargetASC = TargetASC;

				// 应用伤害效果
				UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);
			}
		}
	}
}
