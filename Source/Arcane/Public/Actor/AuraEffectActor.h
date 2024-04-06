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

	// �ص��¼�
	UFUNCTION()
	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,	//	�ص����
		AActor* OtherActor,							//	�ص���Actor
		UPrimitiveComponent* OtherComp,				//	�ص������
		int32 OtherBodyIndex,						//	�ص���BodyIndex
		bool bFromSweep,							//	�Ƿ���ɨ��
		const FHitResult& SweepResult);				//	ɨ����

	UFUNCTION()
	virtual void EndOverlap(
		UPrimitiveComponent* OverlappedComponent,	//	�ص����
		AActor* OtherActor,							//	�ص���Actor
		UPrimitiveComponent* OtherComp,				//	�ص������
		int32 OtherBodyIndex);						//	�ص���BodyIndex

protected:
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USphereComponent> Sphere;	// ������ײ���

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> StaticMesh;	// ��̬�������

};
