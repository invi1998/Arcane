// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;		// ¿Ø¼þ¿ØÖÆÆ÷
	
};
