// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/GDUserWidget.h"
#include "UI/WidgetController/GDWidgetController.h"

void UGDUserWidget::SetWidgetController(UObject* InWidgetController)
{
	if (!InWidgetController)
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget Controller Is Null"));
		return;
	}
	
	WidgetController = InWidgetController;
	// 通知控制器设置完成
	OnWidgetControllerSet();
}
