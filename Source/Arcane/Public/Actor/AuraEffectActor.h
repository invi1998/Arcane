// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

UCLASS()
class ARCANE_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

	// 重叠事件
	UFUNCTION()
	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,	//	重叠组件
		AActor* OtherActor,							//	重叠的Actor
		UPrimitiveComponent* OtherComp,				//	重叠的组件
		int32 OtherBodyIndex,						//	重叠的BodyIndex
		bool bFromSweep,							//	是否是扫描
		const FHitResult& SweepResult);				//	扫描结果

	UFUNCTION()
	virtual void EndOverlap(
		UPrimitiveComponent* OverlappedComponent,	//	重叠组件
		AActor* OtherActor,							//	重叠的Actor
		UPrimitiveComponent* OtherComp,				//	重叠的组件
		int32 OtherBodyIndex);						//	重叠的BodyIndex

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> Sphere;	// 球形碰撞组件

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMesh;	// 静态网格组件

};
