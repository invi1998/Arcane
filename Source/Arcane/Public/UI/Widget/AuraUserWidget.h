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
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* Controller);		// ���ÿؼ�������

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;		// �ؼ�������

protected:
	UFUNCTION(BlueprintImplementableEvent)		// ��ͼʵ���¼����������ÿؼ�������
	void WidgetControllerSet();		// ���ÿؼ�������
	
};
