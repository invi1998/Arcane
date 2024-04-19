// Copyright INVI1998


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const APawn* OwnerPawn = AIOwner->GetPawn();

	// 获取所有玩家, 通过Tag来区分敌人和玩家
	const FName TargetTag = OwnerPawn->ActorHasTag("Player") ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;	// 用来存储所有拥有Tag的Actor
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);	// 获取所有拥有Tag的Actor

	float ClosestDistance = FLT_MAX;	// 最近的距离
	AActor* ClosestActor = nullptr;		// 最近的Actor

	// 遍历所有拥有Tag的Actor，找到最近的一个
	for (AActor* Actor : ActorsWithTag)
	{
		if (!IsValid(Actor) || Actor == OwnerPawn)
		{
			continue;
		}
		const float Distance = OwnerPawn->GetDistanceTo(Actor);

		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	// 设置黑板值（将最近的Actor设置为要跟随的目标）
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);

}
