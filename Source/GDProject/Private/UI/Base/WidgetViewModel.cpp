// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/WidgetViewModel.h"
#include "UI/Base/UISubsystem.h"
#include "GameFramework/Character.h"

void UWidgetViewModel::Initialize(APlayerController* Owner) {
	check(IsValid(Owner));
	PlayerController = Owner;
	Character = PlayerController->GetCharacter();
	OnInitialized();
}

void UWidgetViewModel::Deinitialize() {
	OnDeinitialized();
}

UUISubsystem* UWidgetViewModel::GetUISubsystem() const {
	check(PlayerController.IsValid());
	return PlayerController->GetLocalPlayer()->GetSubsystem<UUISubsystem>();
}
