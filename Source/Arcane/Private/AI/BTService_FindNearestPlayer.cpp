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

	// ��ȡ�������, ͨ��Tag�����ֵ��˺����
	const FName TargetTag = OwnerPawn->ActorHasTag("Player") ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;	// �����洢����ӵ��Tag��Actor
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TargetTag, ActorsWithTag);	// ��ȡ����ӵ��Tag��Actor

	float ClosestDistance = FLT_MAX;	// ����ľ���
	AActor* ClosestActor = nullptr;		// �����Actor

	// ��������ӵ��Tag��Actor���ҵ������һ��
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

	// ���úڰ�ֵ���������Actor����ΪҪ�����Ŀ�꣩
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);

}
