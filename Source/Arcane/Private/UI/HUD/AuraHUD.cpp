// Copyright INVI1998


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	
	if (!OverlayWidgetController)
	{
		checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// 如果OverlayWidgetControllerClass为空，那么就会报错
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is nullptr! please fill out BP_AuraHUD"));		// 如果OverlayWidgetClass为空，那么就会报错
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// 如果OverlayWidgetControllerClass为空，那么就会报错
	
	UUserWidget* TempWidget= CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);	// 创建一个OverlayWidget
	OverlayWidget = Cast<UAuraUserWidget>(TempWidget);	// 将这个OverlayWidget转换为UAuraUserWidget

	const FWidgetControllerParams WCParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);	// 创建一个控制器参数
	UOverlayWidgetController* TempController = GetOverlayWidgetController(WCParams);	// 获取一个OverlayWidgetController

	OverlayWidget->SetWidgetController(TempController);	// 设置OverlayWidget的控制器

	OverlayWidget->AddToViewport();
}
