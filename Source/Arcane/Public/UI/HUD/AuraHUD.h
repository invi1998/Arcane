// Copyright INVI1998

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
struct FWidgetControllerParams;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class UAttributeMenuWidgetController;

/**
 * 
 */
UCLASS()
class ARCANE_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

protected:

private:
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;		// ȫ��ֻ����һ��OverlayWidget������������Ҫ����ʵ��Ϊһ������

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;	// ���������������OverlayWidget��

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;		// ���������������OverlayWidgetController��

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;		// ȫ��ֻ����һ��OverlayWidgetController������������Ҫ����ʵ��Ϊһ������

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;	// ���Բ˵��Ŀ�����

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;	// ���Բ˵��Ŀ�������
};
