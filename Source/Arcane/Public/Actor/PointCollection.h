// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class ARCANE_API APointCollection : public AActor
{
	GENERATED_BODY()
	
public:	
	APointCollection();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure)
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation, int32 NumPoints, float YawOverride = 0.f);		// 获取地面点

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<USceneComponent*> ImmutablePoints;	// 不可变点集合

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_0;		// 生成点0

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_1;		// 生成点1

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_2;		// 生成点2

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_3;		// 生成点3

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_4;		// 生成点4

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_5;		// 生成点5

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_6;		// 生成点6

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_7;		// 生成点7

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_8;		// 生成点8

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_9;		// 生成点9

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PointCollection")
	TObjectPtr<USceneComponent> Pt_10;		// 生成点10

};
