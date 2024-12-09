// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventorySubsystem.h"

#include "GDLog.h"
#include "Inventory/InventoryItemInstance.h"

void UInventorySubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	GDLOG(TEXT("InventorySubsystem"), TEXT("Initialize"));
}

void UInventorySubsystem::Deinitialize() {
	Super::Deinitialize();
	GDLOG(TEXT("InventorySubsystem"), TEXT("Deinitialize"));
}

UInventoryItemInstance* UInventorySubsystem::GenerateItemInstance(UInventoryItemDefinition* ItemDefinition) const {
	if (!ItemDefinition) {
		GDLOG(TEXT("InventorySubsystem"), TEXT("ItemDefinition Is Invalid"));
		return nullptr;
	}
	UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>();
	ItemInstance->SetItemDifinition(ItemDefinition);
	return ItemInstance;
}
