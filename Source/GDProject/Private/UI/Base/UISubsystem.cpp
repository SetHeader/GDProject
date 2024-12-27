// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/UISubsystem.h"

#include "UI/Base/WidgetController.h"
#include "UI/Base/WidgetViewModel.h"

void UUISubsystem::InitializeSubsystem() {
	if (bInitialized) return;
	
	bInitialized = true;
	OnInitedDelegate.Broadcast(this);
}

void UUISubsystem::PlayerControllerChanged(APlayerController* NewPlayerController) {
	Super::PlayerControllerChanged(NewPlayerController);
	if (CurrentPlayerController != NewPlayerController) {
		WidgetControllers.Empty();
		WidgetViewModels.Empty();
		CurrentPlayerController = NewPlayerController;
	}
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

UWidgetViewModel* UUISubsystem::FindOrCreateWidgetViewModel(TSubclassOf<UWidgetViewModel> WidgetViewModelClass) {
	TObjectPtr<UWidgetViewModel>* WidgetViewModel = WidgetViewModels.Find(WidgetViewModelClass);
	
	if (!WidgetViewModel) {
		UWidgetViewModel* WidgetViewModelTemp = CreateWidgetViewModel(WidgetViewModelClass);
		if (WidgetViewModelTemp) {
			WidgetViewModelTemp->Initialize(CurrentPlayerController.Get());
			WidgetViewModels.Add(WidgetViewModelClass, WidgetViewModelTemp);
			return WidgetViewModelTemp;
		}
	}
	
	return WidgetViewModel ? *WidgetViewModel : nullptr;
}

UWidgetViewModel* UUISubsystem::CreateWidgetViewModel(TSubclassOf<UWidgetViewModel> WidgetViewModelClass) const {
	return NewObject<UWidgetViewModel>(CurrentPlayerController.Get(), WidgetViewModelClass);
}
