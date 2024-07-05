// Copyright INVI1998


#include "Actor/MonkSwingProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

void AMonkSwingProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AMonkSwingProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto& Pair : DamageEffectParams)
	{
		FDamageEffectParams& Params = Pair.Value;

		if (IsNotValidOverlap(OtherActor, Params)) return;

		if (HasAuthority())
		{
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
			{
				Params.TargetASC = TargetASC;

				UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);
			}
		}
	}
}

void AMonkSwingProjectile::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParameters;

		CueParameters.Location = GetActorLocation();

		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_Swing, CueParameters);
	}

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bHit = true;
}
