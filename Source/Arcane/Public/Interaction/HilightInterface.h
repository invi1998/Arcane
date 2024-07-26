// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HilightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
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
	virtual void HighlightActor() = 0;		// 高亮显示
	virtual void UnHighlightActor() = 0;	// 取消高亮显示
};
