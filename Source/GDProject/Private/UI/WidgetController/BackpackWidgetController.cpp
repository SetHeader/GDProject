// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BackpackWidgetController.h"

#include "InventoryStatics.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"

void UBackpackWidgetController::OnInitialized() {
	OnItemSlotChangedHandle =
		InventoryManagerComponent->OnItemSlotUpdatedDelegate.AddLambda([this](const FItemSlotChangedData& ChangedData) {
			FItemSlotInfo SlotInfo = UInventoryStatics::GetItemSlotInfo(ChangedData.SlotHandle);
			this->OnSlotInfoChangedDelegate.Broadcast(SlotInfo);
		});
}

void UBackpackWidgetController::OnDeinitialized() {
	InventoryManagerComponent->OnItemSlotUpdatedDelegate.Remove(OnItemSlotChangedHandle);
}

TArray<FInventoryItemSlot> UBackpackWidgetController::GetAllItemSlots() {
	return InventoryManagerComponent.IsValid() ? InventoryManagerComponent->GetAllItemSlots() : TArray<FInventoryItemSlot>();
}

TArray<FInventoryItemSlotHandle> UBackpackWidgetController::GetAllItemSlotHandles() {
	return InventoryManagerComponent.IsValid() ? InventoryManagerComponent->GetAllItemSlotHandles() : TArray<FInventoryItemSlotHandle>();
}

void UBackpackWidgetController::UseItem(UAbilitySystemComponent* TargetASC,
							 FInventoryItemSlotHandle SlotHandle,
							 int32 AmountToUse) {
	UInventoryStatics::UseItem(TargetASC, SlotHandle, AmountToUse);
}

void UBackpackWidgetController::EquipItem(FInventoryItemSlotHandle SlotHandle) {
	FInventoryItemSlot ItemSlot = InventoryManagerComponent->GetItemSlot(SlotHandle);
	
	FInventoryItemSlotHandle EquipmentSlotHandle = EquipmentManagerComponent->EquipItem(ItemSlot.ItemInstance);
	if (EquipmentSlotHandle.IsValid()) {
		int32 StackCount = EquipmentSlotHandle.GetStackCount();
		FInventoryRemovedItemInfo RemovedItemInfo;
		InventoryManagerComponent->RemoveItemFromSlot(SlotHandle, StackCount, RemovedItemInfo);
	}
}

void UBackpackWidgetController::MoveItem(FInventoryItemSlotHandle FromSlotHandle,
	FInventoryItemSlotHandle ToSlotHandle,
	int32 AmountToMove) {
	UInventoryStatics::MoveItem(FromSlotHandle, ToSlotHandle, AmountToMove);
}

void UBackpackWidgetController::OnRightClicked(UUserWidget* UserWidget, FInventoryItemSlotHandle SlotHandle) {
	PreviousUserWidget = CurrentUserWidget;
	CurrentUserWidget = UserWidget;
	CurrentItemSlotHandle = SlotHandle;
	CurrentItemSlotInfo = UInventoryStatics::GetItemSlotInfo(SlotHandle);
	OnRightClickedDelegate.Broadcast();
}
