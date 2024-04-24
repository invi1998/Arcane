// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;	// ����ÿ���ٻ�����֮��ļ��

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);	// ��תSpawnSpread�Ƕ�/2, �õ���ߵ�����
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);	// ��ת-SpawnSpread�Ƕ�/2, �õ��ұߵ�����

	//// debug DrawDebugArrow start
	//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FColor::Purple, 10.f);
	//DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 10.f, 12, FColor::Purple, false, 10.f);
	//DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MinSpawnDistance, 10.f, 12, FColor::Purple, false, 10.f);
	//
	//UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 4.f, FColor::Orange, 10.f);
	//DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 10.f, 12, FColor::Orange, false, 10.f);
	//DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 10.f, 12, FColor::Orange, false, 10.f);
	//// debug DrawDebugArrow end

	TArray<FVector> OutSpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector ChosenSpawnLocation = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector) * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance) + Location;	// ����λ��
		//// debug DrawDebugSphere start
		// DrawDebugSphere(GetWorld(), SpawnLocation, 5.f, 5, FColor::Green, false, 10.f);
		//// debug DrawDebugSphere end

		// ͬʱ�����ǻ���Ҫ�������λ���Ƿ�Ϸ��������Ƿ��ڵ����ϣ��Ƿ���NavMesh�ϵ�
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, ChosenSpawnLocation, ChosenSpawnLocation - FVector::UpVector * 1000.f, ECollisionChannel::ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			// ��������ˣ���ô������Ҫ����һ��λ�ã����������ƶ�һ��
			OutSpawnLocations.Add(HitResult.Location + FVector::UpVector * 2.f);
		}
		else
		{
			OutSpawnLocations.Add(ChosenSpawnLocation);
		}
	}

	return OutSpawnLocations;
}
