// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GDHUD.h"
#include "UI/Widget/GDUserWidget.h"
#include "UI/WidgetController/GDAttributeMenuWidgetController.h"
#include "UI/WidgetController/GDOverlayWidgetController.h"
#include "UI/WidgetController/GDWidgetController.h"

UGDOverlayWidgetController* AGDHUD::GetOverlayWidgetController() const
{
	return OverlayWidgetController;
}

UGDAttributeMenuWidgetController* AGDHUD::GetAttributeMenuWidgetController() const
{
	return AttributeMenuWidgetController;
}

void AGDHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass 未定义"));
	checkf(OverlayWidgetControllerClass, TEXT("WidgetControllerClass 未定义"));
	checkf(AttributeMenuWidgetControllerClass, TEXT("WidgetControllerClass 未定义"));
	
	OverlayWidgetController = NewObject<UGDOverlayWidgetController>(this, OverlayWidgetControllerClass);
	OverlayWidgetController->SetParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	
	OverlayWidget = CreateWidget<UGDUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget->SetWidgetController(OverlayWidgetController);
	// 通知属性初始化
	OverlayWidgetController->BroadcastInitialValues();
	
	AttributeMenuWidgetController = NewObject<UGDAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
	AttributeMenuWidgetController->SetParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	
	OverlayWidget->AddToViewport();
}
