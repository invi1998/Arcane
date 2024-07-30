// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HilightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UHilightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCANE_API IHilightInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void HighlightActor();		// 高亮显示

	UFUNCTION(BlueprintNativeEvent)
	void UnHighlightActor();	// 取消高亮显示

	UFUNCTION(BlueprintNativeEvent)
	void SetMoveToLocation(FVector& OutLocation);	// 设置移动到的位置

};
