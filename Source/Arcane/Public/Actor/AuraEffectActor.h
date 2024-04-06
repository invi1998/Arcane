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

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;	// 实例化的GameplayEffect类

	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);	// 将效果应用到目标，传入目标和GameplayEffect类

private:
};
