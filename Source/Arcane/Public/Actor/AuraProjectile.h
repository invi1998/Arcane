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
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;	// Ͷ�����ƶ����

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))	// ExposeOnSpawn ��ʾ�����ڴ���ʵ��ʱ���ø�����
	FGameplayEffectSpecHandle DamageEffectSpecHandle;	// �˺�Ч�����

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	// ������ײ���ص��¼�
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bHit = false;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 5.0f;	// ����ʱ��

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;	// ������ײ��

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;	// ������Ч

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;		// ������Ч

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;		// ������Ч

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;		// ������Ч���

};
