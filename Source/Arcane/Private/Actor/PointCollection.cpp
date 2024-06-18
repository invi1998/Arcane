// Copyright INVI1998


#include "Actor/PointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = true;

	Pt_0 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_0"));
	ImmutablePoints.Add(Pt_0);
	SetRootComponent(Pt_0);

	Pt_1 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_1"));
	ImmutablePoints.Add(Pt_1);
	Pt_1->SetupAttachment(GetRootComponent());

	Pt_2 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_2"));
	ImmutablePoints.Add(Pt_2);
	Pt_2->SetupAttachment(GetRootComponent());

	Pt_3 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_3"));
	ImmutablePoints.Add(Pt_3);
	Pt_3->SetupAttachment(GetRootComponent());

	Pt_4 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_4"));
	ImmutablePoints.Add(Pt_4);
	Pt_4->SetupAttachment(GetRootComponent());

	Pt_5 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_5"));
	ImmutablePoints.Add(Pt_5);
	Pt_5->SetupAttachment(GetRootComponent());

	Pt_6 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_6"));
	ImmutablePoints.Add(Pt_6);
	Pt_6->SetupAttachment(GetRootComponent());

	Pt_7 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_7"));
	ImmutablePoints.Add(Pt_7);
	Pt_7->SetupAttachment(GetRootComponent());

	Pt_8 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_8"));
	ImmutablePoints.Add(Pt_8);
	Pt_8->SetupAttachment(GetRootComponent());

	Pt_9 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_9"));
	ImmutablePoints.Add(Pt_9);
	Pt_9->SetupAttachment(GetRootComponent());

	Pt_10 = CreateDefaultSubobject<USceneComponent>(TEXT("Pt_10"));
	ImmutablePoints.Add(Pt_10);
	Pt_10->SetupAttachment(GetRootComponent());

}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride)
{
	TArray<USceneComponent*> Result;
	if (NumPoints <= 0)
	{
		return Result;
	}

	if (NumPoints > ImmutablePoints.Num())
	{
		NumPoints = ImmutablePoints.Num();
	}

	for (USceneComponent* Point : ImmutablePoints)
	{
		if (Result.Num() >= NumPoints)
		{
			break;
		}
		if (Point != Pt_0)
		{
			FVector ToPoint = Point->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Point->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}

		const FVector RaisedLocation = Point->GetComponentLocation() + FVector(0.f, 0.f, 500.f);		// 提高点
		const FVector LoweredLocation = Point->GetComponentLocation() - FVector(0.f, 0.f, 500.f);	// 降低点

		FHitResult HitResult;
		TArray<AActor*> ActorsToIgnore;
		UAuraAbilitySystemLibrary::GetLivePlayerWithinRadius(this, ActorsToIgnore, TArray<AActor*>(), GetActorLocation(), 1500.f);


		FCollisionQueryParams Params;
		Params.AddIgnoredActors(ActorsToIgnore);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, "BlockAll", Params);	// 碰撞检测，追踪并忽略指定半径内的玩家

		const FVector AdjustedLocation = FVector(Point->GetComponentLocation().X, Point->GetComponentLocation().Y, HitResult.ImpactPoint.Z);

		Point->SetWorldLocation(AdjustedLocation);
		Point->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));		// 调整点的位置和旋转，使其与地面接触，且法线与地面法线一致（比如坡度，就是让点的法线与坡度法线一致）

		Result.Add(Point);
	}

	return Result;
}

