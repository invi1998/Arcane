// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		// 如果没有目标，就取消当前技能
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeamSpell::StoreOwnerPlayer()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();

		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	// 采用球形射线检测
	check(OwnerCharacter);

	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeaponMesh(OwnerCharacter))
		{
			// 获取武器插槽位置
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));

			TArray<AActor*> IgnoredActors;
			IgnoredActors.Add(OwnerCharacter);

			FHitResult HitResult;

			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				IgnoredActors,
				EDrawDebugTrace::None,
				HitResult,
				true,
				FLinearColor::Red,
				FLinearColor::Green,
				5.f);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}

	
}

void UAuraBeamSpell::StoreAdditionalTarget(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(GetAvatarActorFromActorInfo());
	IgnoredActors.Add(MouseHitActor);

	TArray<AActor*> OverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		IgnoredActors,
		MouseHitActor->GetActorLocation(),
		BeamRange
	);

	// 将所有的Actor按照距离MouseHitActor的距离进行排序，将前MaxNumOfTargets个Actor加入到OutAdditionalTargets中
	// const int32 RealNumOfTargets = FMath::Min(GetAbilityLevel(), MaxNumOfTargets);
	const int32 RealNumOfTargets = 5;
	UAuraAbilitySystemLibrary::GetClosestTargets(RealNumOfTargets, OverlappingActors, MouseHitActor->GetActorLocation(), OutAdditionalTargets);
}
