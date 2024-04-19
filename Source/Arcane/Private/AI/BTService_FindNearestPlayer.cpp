// Copyright INVI1998


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwnerPawn = AIOwner->GetPawn();

	// 获取所有玩家, 通过Tag来区分敌人和玩家
	const FName TargetTag = OwnerPawn->ActorHasTag("Player") ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;	// 用来存储所有拥有Tag的Actor
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);	// 获取所有拥有Tag的Actor

	for (AActor* Actor : ActorsWithTag)
	{
		if (Actor == OwnerPawn)
		{
			continue;
		}

		const float Distance = FVector::Dist(OwnerPawn->GetActorLocation(), Actor->GetActorLocation());

		// 获取黑板
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

		// 获取黑板中的距离
		const float DistanceToTarget = Blackboard->GetValueAsFloat(DistanceToTargetSelector.SelectedKeyName);

		// 如果没有距离或者距离比当前距离小
		if (DistanceToTarget == 0 || Distance < DistanceToTarget)
		{
			// 设置黑板中的距离
			Blackboard->SetValueAsFloat(DistanceToTargetSelector.SelectedKeyName, Distance);

			// 设置黑板中的目标
			Blackboard->SetValueAsObject(TargetToFollowSelector.SelectedKeyName, Actor);
		}
	}

}
