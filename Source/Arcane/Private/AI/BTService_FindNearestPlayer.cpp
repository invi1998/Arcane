// Copyright INVI1998


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Find Nearest Player: AIOwner %s"), *AIOwner->GetName()));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Find Nearest Player: ActorOwner %s"), *ActorOwner->GetName()));
}
