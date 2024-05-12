// Copyright INVI1998


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	
	if (!OverlayWidgetController)
	{
		checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// ���OverlayWidgetControllerClassΪ�գ���ô�ͻᱨ��
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();		// �󶨻ص�������������
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!AttributeMenuWidgetController)
	{
		checkf(AttributeMenuWidgetControllerClass, TEXT("AttributeMenuWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// ���AttributeMenuWidgetControllerClassΪ�գ���ô�ͻᱨ��
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();		// �󶨻ص�������������
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (!SpellMenuWidgetController)
	{
		checkf(SpellMenuWidgetControllerClass, TEXT("SpellMenuWidgetControllerClass is nullptr! please fill out BP_AuraHUD"));		// ���SpellMenuWidgetControllerClassΪ�գ���ô�ͻᱨ��
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();		// �󶨻ص�������������
	}
	return SpellMenuWidgetController;
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

	TempController->BroadcastInitialValues();	// �㲥��ʼֵ

	OverlayWidget->AddToViewport();
}
