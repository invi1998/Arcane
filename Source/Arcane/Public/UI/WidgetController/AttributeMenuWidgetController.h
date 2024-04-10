// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FAuraAttributeInfo;
class UAttributeInfo;

// ����һ���ܹ㲥AuraAttributeSet����ֵ��ί��
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ARCANE_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;		// �㲥��ʼֵ
	virtual void BindCallbacksToDependencies() override;	// �󶨻ص�������������

	UPROPERTY(BlueprintAssignable, Category = "gas|Attributes")		// ��ͼ�ɵ��õ�ί��
	FAttributeInfoSignature AttributeInfoDelegate;	// ������Ϣί��

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AuraAttributeInfo;	// ������Ϣ

};
