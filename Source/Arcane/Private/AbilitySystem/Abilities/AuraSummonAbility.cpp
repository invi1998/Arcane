// Copyright INVI1998


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;	// 计算每个召唤物体之间的间隔

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);	// 旋转SpawnSpread角度/2, 得到左边的向量
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);	// 旋转-SpawnSpread角度/2, 得到右边的向量

	// debug DrawDebugArrow start
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + LeftOfSpread * MaxSpawnDistance, 4.f, FColor::Purple, 10.f);
	DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 10.f, 12, FColor::Purple, false, 10.f);
	DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MinSpawnDistance, 10.f, 12, FColor::Purple, false, 10.f);
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location, Location + RightOfSpread * MaxSpawnDistance, 4.f, FColor::Orange, 10.f);
	DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 10.f, 12, FColor::Orange, false, 10.f);
	DrawDebugSphere(GetWorld(), Location + RightOfSpread * MinSpawnDistance, 10.f, 12, FColor::Orange, false, 10.f);
	// debug DrawDebugArrow end

	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector SpawnLocation = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector) * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance) + Location;	// 生成位置
		// debug DrawDebugSphere start
		DrawDebugSphere(GetWorld(), SpawnLocation, 5.f, 5, FColor::Green, false, 10.f);
		// debug DrawDebugSphere end

		SpawnLocations.Add(SpawnLocation);
	}

	return SpawnLocations;
}
