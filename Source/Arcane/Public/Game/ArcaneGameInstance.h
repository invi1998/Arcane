// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ArcaneGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UArcaneGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FName PlayerStartTag = FName("");

	UPROPERTY()
	FString LoadSlotName = FString();

private:
	
};
