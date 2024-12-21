// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/UISubsystem.h"

#include "UI/Base/WidgetController.h"

void UUISubsystem::InitializeSubsystem() {
	if (bInitialized) return;
	
	bInitialized = true;
	OnInitedDelegate.Broadcast(this);
}

void UUISubsystem::PlayerControllerChanged(APlayerController* NewPlayerController) {
	Super::PlayerControllerChanged(NewPlayerController);
	CurrentPlayerController = NewPlayerController;
}

UWidgetController* UUISubsystem::FindOrCreateWidgetController(TSubclassOf<UWidgetController> WidgetControllerClass) {
	TObjectPtr<UWidgetController>* WidgetController = WidgetControllers.Find(WidgetControllerClass);
	
	if (!WidgetController) {
		UWidgetController* WidgetControllerTemp = CreateWidgetController(WidgetControllerClass);
		if (WidgetControllerTemp) {
			WidgetControllerTemp->Initialize(CurrentPlayerController.Get());
			WidgetControllers.Add(WidgetControllerClass, WidgetControllerTemp);
			return WidgetControllerTemp;
		}
	}
	
	return WidgetController ? *WidgetController : nullptr;
}

UWidgetController* UUISubsystem::CreateWidgetController(TSubclassOf<UWidgetController> WidgetControllerClass) const {
	return NewObject<UWidgetController>(CurrentPlayerController.Get(), WidgetControllerClass);
}
