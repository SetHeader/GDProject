// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Base/WidgetController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "InventoryStatics.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerState.h"
#include "Inventory/InventoryManagerComponent.h"
#include "AttributeSet.h"


void UWidgetController::Initialize(APlayerController* Owner) {
	check(IsValid(Owner));
	PlayerController = Owner;
	Character = PlayerController->GetCharacter();
	if (Character.IsValid()) {
		PlayerState = Character->GetPlayerState();
		AbilitySystemComponent	= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character.Get());
		AttributeSet = AbilitySystemComponent->GetAttributeSet(UGDAttributeSet::StaticClass());
		InventoryManagerComponent = UInventoryStatics::GetInventoryManagerComponent(Character.Get());
		EquipmentManagerComponent = UInventoryStatics::GetEquipmentManagerComponent(Character.Get());
	}
	OnInitialized();
}

void UWidgetController::Deinitialize() {
	OnDeinitialized();
}
