// Copyright INVI1998


#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameplayCueManager.h"
#include "Components/AudioComponent.h"

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();

	StartOutgoingTimeLine();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AAuraFireBall::OnHit()
{
	// 如何执行本地GameplayCue？
	if (GetOwner())
	{
		// 播放击中特效, 播放击中音效
		FGameplayCueParameters CueParameters;

		CueParameters.Location = GetActorLocation();

		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParameters);

	}

	// 停止飞行音效
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}
