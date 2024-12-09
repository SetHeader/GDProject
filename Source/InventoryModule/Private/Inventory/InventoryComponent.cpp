// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "Inventory/InventoryItemInstance.h"

bool UInventoryComponent::IsValidItemSlot(const FInventoryItemSlotHandle& Handle) const {
	for (const auto& SlotHandle : SlotHandles) {
		if (SlotHandle == Handle) 
			return true;
	}
	return false;
}

FInventoryItemSlot& UInventoryComponent::GetItemSlot(const FInventoryItemSlotHandle& Handle) {
	check(IsValidItemSlot(Handle))
	
	for (auto& Slot : SlotArray.Slots) {
		if (Slot == Handle)
			return Slot;
	}
	
	return SlotArray.Slots[0];
}

bool UInventoryComponent::LootItem(UInventoryItemInstance* Item) {
	if (!IsValid(Item))
		return false;
	
	for (const auto& Slot : SlotArray.Slots) {
		if (Slot.ItemInstance) 
			continue;

		FInventoryItemSlotHandle SlotHandle(Slot, this);
			
		if (AcceptItem(Item, SlotHandle) && PlaceItemIntoSlot(Item, SlotHandle)) 
			return true;
	}

	return false;
}

bool UInventoryComponent::PlaceItemIntoSlot(UInventoryItemInstance* Item,
	const FInventoryItemSlotHandle& SlotHandle) {

	if (!IsValid(Item) || !IsValidItemSlot(SlotHandle)) 
		return false;

	FInventoryItemSlot& Slot = GetItemSlot(SlotHandle);
	UInventoryItemInstance* PreviousItem = Slot.ItemInstance;
	Slot.ItemInstance = Item;
	OnInventoryUpdatedDelegate.Broadcast(this);
	OnItemSlotUpdatedDelegate.Broadcast(this, SlotHandle, Item, PreviousItem);
	return true;
}

bool UInventoryComponent::AcceptItem(UInventoryItemInstance* Item, const FInventoryItemSlotHandle& SlotHandle) {
	if (!IsValid(Item) || !IsValidItemSlot(SlotHandle)) 
		return false;

	return GetItemSlot(SlotHandle).ItemInstance == nullptr;
}

void UInventoryComponent::CreateInventorySlot(const FGameplayTagContainer& SlotTags,
                                                     const FInventoryItemSlotFilter& Filter) {
	FInventoryItemSlot Slot;
	Slot.SlotTags = SlotTags;
	Slot.ItemSlotFilter = Filter;
	Slot.SlotId = ++IdCounter;
	Slot.Owner = this;

	SlotArray.Slots.Add(Slot);
	PostInventoryUpdate();
}

void UInventoryComponent::RemoveInventorySlot(const FInventoryItemSlotHandle& Handle) {
	if (IsValidItemSlot(Handle)) {
		SlotArray.Slots.Remove(GetItemSlot(Handle));
	}
}

void UInventoryComponent::PostInventoryUpdate() {
	SlotHandles.Empty();
	PopulateSlotReferenceArray(SlotHandles);
}

void UInventoryComponent::PopulateSlotReferenceArray(TArray<FInventoryItemSlotHandle>& Handles) {
	for (const auto& Slot : SlotArray.Slots) {
		FInventoryItemSlotHandle Handle(Slot, this);
		Handles.Add(MoveTemp(Handle));
	}
}

bool UInventoryComponent::RemoveItemFromInventory(const FInventoryItemSlotHandle& SlotHandle) {
	if (!IsValidItemSlot(SlotHandle))
		return false;

	FInventoryItemSlot& Slot = GetItemSlot(SlotHandle);
	UInventoryItemInstance* PreviousItem = Slot.ItemInstance;
	if (!IsValid(Slot.ItemInstance))
		return true;

	Slot.ItemInstance = nullptr;
	OnInventoryUpdatedDelegate.Broadcast(this);
	OnItemSlotUpdatedDelegate.Broadcast(this, SlotHandle, Slot.ItemInstance, PreviousItem);
	return true;
}

bool UInventoryComponent::RemoveAllItemFromInventory(TArray<UInventoryItemInstance*>& OutItemsRemoved) {
	for (auto& Slot : SlotArray.Slots) {
		if (IsValid(Slot.ItemInstance)) {
			UInventoryItemInstance* PreviousItem = Slot.ItemInstance;
			OutItemsRemoved.Add(Slot.ItemInstance);
			Slot.ItemInstance = nullptr;

			OnItemSlotUpdatedDelegate.Broadcast(this, FInventoryItemSlotHandle(Slot, this), Slot.ItemInstance, PreviousItem);
		}
	}

	OnInventoryUpdatedDelegate.Broadcast(this);
	return true;
}

TArray<FInventoryItemSlotHandle> UInventoryComponent::GetAllSlotHandles() {
	return SlotHandles;
}

UInventoryItemInstance* UInventoryComponent::GetItemInstanceInSlot(const FInventoryItemSlotHandle& SlotHandle) {
	if (IsValidItemSlot(SlotHandle))
		return GetItemSlot(SlotHandle).ItemInstance;
	return nullptr;
}
