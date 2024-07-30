// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Arcane/Arcane.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HilightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

/**
 * 检查点
 */
UCLASS()
class ARCANE_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHilightInterface
{
	GENERATED_BODY()

public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);	// 构造函数

	// SaveGame 表示是否保存到存档，SaveGame 修饰符告诉 Unreal Engine 该属性应该被保存到游戏存档中。
	// 当游戏保存时，标记为 SaveGame 的属性会被序列化并保存下来，以便下次游戏加载时能够恢复这些状态。
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;	// 是否到达

	/*
	 * Save Interface Start
	 */
	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadActor_Implementation() override;
	/*
	 * Save Interface End
	 */

protected:
	virtual void BeginPlay() override;	// 开始游戏时调用

	/*
	*` Hilight Interface Start
	*/
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutLocation) override;
	/*
	 * Hilight Interface End
	 */

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;	// 移动到组件

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	// 碰撞体重叠事件

	UFUNCTION(BlueprintImplementableEvent)	// 蓝图实现的事件
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialIns);	// 检查点到达

	void HandleGlowEffects();	// 处理光效

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;	// 检查点模型

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 CustomDepthStencilValueOverride = CUSTOM_DEPTH_STENCIL_TAN;	// 自定义深度缓冲值
	

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UShapeComponent> CollisionSphere;	// 碰撞体


	
};
