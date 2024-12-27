// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/SimpleUserWidget.h"

#include "UI/Base/UISubsystem.h"
#include "UI/Base/WidgetController.h"
#include "UI/Base/WidgetViewModel.h"

bool USimpleUserWidget::Initialize() {
	if (!Super::Initialize()) 
		return false;
	
	if (auto LocalPlayer = GetOwningLocalPlayer()) {
		UUISubsystem* UISubsystem = ULocalPlayer::GetSubsystem<UUISubsystem>(LocalPlayer);
		if (UISubsystem) 
			if (UISubsystem->IsInitialized()) 
				OnUISubsystemInited(UISubsystem);
			else 
				UISubsystem->OnInitedDelegate.AddUObject(this, &USimpleUserWidget::OnUISubsystemInited);
	}
	return true;
}

void USimpleUserWidget::BeginDestroy() {
	Super::BeginDestroy();
	if (bAlwaysCreateNewWidgetController && WidgetController) {
		WidgetController->Deinitialize();
	}
	if (bAlwaysCreateNewWidgetViewModel && WidgetViewModel) {
		WidgetViewModel->Deinitialize();
	}
}

void USimpleUserWidget::OnUISubsystemInited(UUISubsystem* InSubsystem) {
	if (WidgetControllerClass) {
		if (bAlwaysCreateNewWidgetController)
			WidgetController = InSubsystem->CreateWidgetController(WidgetControllerClass);
		else
			WidgetController = InSubsystem->FindOrCreateWidgetController(WidgetControllerClass);
	
		if (WidgetController)
			OnWidgetControllerSet();
	}

	if (WidgetViewModelClass) {
		if (bAlwaysCreateNewWidgetViewModel)
			WidgetViewModel = InSubsystem->CreateWidgetViewModel(WidgetViewModelClass);
		else
			WidgetViewModel = InSubsystem->FindOrCreateWidgetViewModel(WidgetViewModelClass);
		
		if (WidgetViewModel)
			OnWidgetViewModelSet();
	}

	OnWidgetControllerOrViewModelSet();
	InSubsystem->OnInitedDelegate.RemoveAll(this);
}

UUISubsystem* USimpleUserWidget::GetUISubsystem() const {
	check(GetOwningLocalPlayer());
	return GetOwningLocalPlayer()->GetSubsystem<UUISubsystem>();
}
