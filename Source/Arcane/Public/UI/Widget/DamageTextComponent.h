// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * …À∫¶ ˝œ‘
 */
UCLASS()
class ARCANE_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetDamageText(float Damage, bool bBlockedHit, bool bCriticalHit);
	
};
