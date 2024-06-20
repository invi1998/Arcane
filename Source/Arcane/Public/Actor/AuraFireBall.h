// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * 使用Timeline来制作一个围绕角色绕圈散开并收拢的飞弹技能
 */
UCLASS()
class ARCANE_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void StartOutgoingTimeLine();	// 开始飞行时间线
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void EndOutgoingTimeLine();	// 结束飞行时间线

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<AActor> ReturnTarget;	// 返回目标

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxFlyDistance = 1000.0f;	// 最大飞行距离

protected:

	virtual void BeginPlay() override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
};
