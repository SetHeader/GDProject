// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemFragment.h"

#include "Inventory/InventoryItemInstance.h"

void UInventoryItemFragment::OnInstancedCreated(UInventoryItemInstance* Instance) {
	this->OwnerItemInstance = Instance;
}

bool UInventoryItemFragment::operator==(const UInventoryItemFragment& Other) const {
	return this == &Other;
}

bool UInventoryItemFragment::operator!=(const UInventoryItemFragment& Other) const {
	return !operator==(Other);
}
