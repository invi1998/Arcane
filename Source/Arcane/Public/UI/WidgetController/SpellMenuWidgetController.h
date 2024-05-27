// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ARCANE_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;	// �㲥��ʼֵ

	virtual void BindCallbacksToDependencies() override;	// �󶨻ص�������������

	UPROPERTY(BlueprintAssignable, Category = "gas|Attributes")		// ��ͼ�ɵ��õ�ί��
	FOnPlayerStatChangeSignatureInt SkillPointChangeDelegate;	// ���ܵ�ı�ί��
	
};
