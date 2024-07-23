// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Checkpoint.generated.h"

/**
 * 检查点
 */
UCLASS()
class ARCANE_API ACheckpoint : public APlayerStart
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);	// 构造函数

protected:
	virtual void BeginPlay() override;	// 开始游戏时调用

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	// 碰撞体重叠事件

	

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;	// 检查点模型

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShapeComponent> CollisionSphere;	// 碰撞体
	
};
