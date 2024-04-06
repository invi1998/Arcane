// Copyright INVI1998


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (OverlayWidgetClass)
	{
		OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
		if (OverlayWidget)
		{
			OverlayWidget->AddToViewport();
		}
	}
}
