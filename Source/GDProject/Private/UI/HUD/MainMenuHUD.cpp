// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Base/SimpleUserWidget.h"
#include "UI/Base/UISubsystem.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

class UUISubsystem;

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	UUISubsystem* Subsystem = GetOwningPlayerController()->GetLocalPlayer()->GetSubsystem<UUISubsystem>();
	Subsystem->InitializeSubsystem();
	
	if (MainMenuWidgetClass) {
		MainMenuWidget = CreateWidget<USimpleUserWidget>(GetOwningPlayerController(), MainMenuWidgetClass);
		MainMenuWidget->AddToViewport();
	}
}
