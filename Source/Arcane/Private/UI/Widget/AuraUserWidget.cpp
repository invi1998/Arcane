// Copyright INVI1998


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* Controller)
{
	WidgetController = Controller;
	WidgetControllerSet();
}
