// Copyright INVI1998


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	
	if (!OverlayWidgetController)
	{
		checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// ���OverlayWidgetControllerClassΪ�գ���ô�ͻᱨ��
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is nullptr! please fill out BP_AuraHUD"));		// ���OverlayWidgetClassΪ�գ���ô�ͻᱨ��
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// ���OverlayWidgetControllerClassΪ�գ���ô�ͻᱨ��
	
	UUserWidget* TempWidget= CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);	// ����һ��OverlayWidget
	OverlayWidget = Cast<UAuraUserWidget>(TempWidget);	// �����OverlayWidgetת��ΪUAuraUserWidget

	const FWidgetControllerParams WCParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);	// ����һ������������
	UOverlayWidgetController* TempController = GetOverlayWidgetController(WCParams);	// ��ȡһ��OverlayWidgetController

	OverlayWidget->SetWidgetController(TempController);	// ����OverlayWidget�Ŀ�����

	OverlayWidget->AddToViewport();
}
