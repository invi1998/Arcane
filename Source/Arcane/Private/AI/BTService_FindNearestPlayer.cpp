// Copyright INVI1998


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwnerPawn = AIOwner->GetPawn();

	// ��ȡ�������, ͨ��Tag�����ֵ��˺����
	const FName TargetTag = OwnerPawn->ActorHasTag("Player") ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;	// �����洢����ӵ��Tag��Actor
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);	// ��ȡ����ӵ��Tag��Actor

	for (AActor* Actor : ActorsWithTag)
	{
		if (Actor == OwnerPawn)
		{
			continue;
		}

		const float Distance = FVector::Dist(OwnerPawn->GetActorLocation(), Actor->GetActorLocation());

		// ��ȡ�ڰ�
		UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

		// ��ȡ�ڰ��еľ���
		const float DistanceToTarget = Blackboard->GetValueAsFloat(DistanceToTargetSelector.SelectedKeyName);

		// ���û�о�����߾���ȵ�ǰ����С
		if (DistanceToTarget == 0 || Distance < DistanceToTarget)
		{
			// ���úڰ��еľ���
			Blackboard->SetValueAsFloat(DistanceToTargetSelector.SelectedKeyName, Distance);

			// ���úڰ��е�Ŀ��
			Blackboard->SetValueAsObject(TargetToFollowSelector.SelectedKeyName, Actor);
		}
	}

}
