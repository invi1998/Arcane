// Copyright INVI1998


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "Player/AuraPlayerController.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	// 创建一个UTargetDataUnderMouse*类型的指针
	UTargetDataUnderMouse* MyObj = NewObject<UTargetDataUnderMouse>(OwningAbility);
	// 返回这个指针
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	AAuraPlayerController* PC = Cast<AAuraPlayerController>(Ability->GetCurrentActorInfo()->PlayerController.Get());
	if (PC)
	{
		ValidData.Broadcast(PC->GetCursorHitResult().Location);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UTargetDataUnderMouse::Activate: PlayerController is NULL"));
	}
}
