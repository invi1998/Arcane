// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class ARCANE_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;	// 投射物移动组件

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))	// ExposeOnSpawn 表示可以在创建实例时设置该属性
	FGameplayEffectSpecHandle DamageEffectSpecHandle;	// 伤害效果句柄

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	// 球形碰撞体重叠事件
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bHit = false;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5.0f;	// 生存时间

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;	// 球形碰撞体

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;	// 击中特效

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;		// 击中音效

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;		// 飞行音效

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;		// 飞行音效组件

};
