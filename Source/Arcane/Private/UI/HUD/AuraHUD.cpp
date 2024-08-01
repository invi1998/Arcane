// Copyright INVI1998


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Game/ArcaneGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	
	if (!OverlayWidgetController)
	{
		checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// 如果OverlayWidgetControllerClass为空，那么就会报错
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();		// 绑定回调函数到依赖项
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!AttributeMenuWidgetController)
	{
		checkf(AttributeMenuWidgetControllerClass, TEXT("AttributeMenuWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// 如果AttributeMenuWidgetControllerClass为空，那么就会报错
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();		// 绑定回调函数到依赖项
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!SpellMenuWidgetController)
	{
		checkf(SpellMenuWidgetControllerClass, TEXT("SpellMenuWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// 如果SpellMenuWidgetControllerClass为空，那么就会报错
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();		// 绑定回调函数到依赖项
	}
	return SpellMenuWidgetController;
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

	TempController->BroadcastInitialValues();	// 广播初始值

	OverlayWidget->AddToViewport();

	if (const UArcaneGameInstance *GameInstance = Cast<UArcaneGameInstance>(GetGameInstance()))
	{
		GameInstance->OnOverlayWidgetShow.Broadcast();		// 广播OverlayWidget显示的委托
	}

}

void AAuraHUD::ShowOverlayWidget()
{
	if (OverlayWidget)
	{
		/*OverlayWidget->AddToViewport();

		UOverlayWidgetController* TempController = Cast<UOverlayWidgetController>(OverlayWidget->WidgetController);
		TempController->BroadcastInitialValues();
		TempController->BroadcastAbilityInfo();*/

		OverlayWidget->SetRenderOpacity(1.0f);
	}
}

void AAuraHUD::HideOverlayWidget()
{
	if (OverlayWidget)
	{
		OverlayWidget->SetRenderOpacity(0.0f);
	}
}
