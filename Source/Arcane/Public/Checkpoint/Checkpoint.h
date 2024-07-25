// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

/**
 * 检查点
 */
UCLASS()
class ARCANE_API ACheckpoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);	// 构造函数

	// SaveGame 表示是否保存到存档，SaveGame 修饰符告诉 Unreal Engine 该属性应该被保存到游戏存档中。
	// 当游戏保存时，标记为 SaveGame 的属性会被序列化并保存下来，以便下次游戏加载时能够恢复这些状态。
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;	// 是否到达

protected:
	virtual void BeginPlay() override;	// 开始游戏时调用

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	// 碰撞体重叠事件

	UFUNCTION(BlueprintImplementableEvent)	// 蓝图实现的事件
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialIns);	// 检查点到达

	void HandleGlowEffects();	// 处理光效
	

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;	// 检查点模型

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShapeComponent> CollisionSphere;	// 碰撞体


	
};
