// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class ARCANE_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:


protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class APlayerController> PlayerController;		// ��ҿ�����

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class APlayerState> PlayerState;		// ���״̬

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class UAbilitySystemComponent> AbilitySystemComponent;		// ����ϵͳ���

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<class UAttributeSet> AttributeSet;		// ���Լ�

};
