// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/BackpackWidgetController.h"

#include "InventoryStatics.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryManagerComponent.h"

void UBackpackWidgetController::OnInitialized() {
	OnItemSlotChangedHandle =
		InventoryManagerComponent->OnItemSlotChanged.AddLambda([this]() {
			this->OnSlotInfoChangedDelegate.Broadcast();
		});

	OnEquipmentSlotChangedHandle =
		EquipmentManagerComponent->OnItemSlotChanged.AddLambda([this]() {
			this->OnEquipmentSlotChangedDelegate.Broadcast();
		});
}

void UBackpackWidgetController::OnDeinitialized() {
	InventoryManagerComponent->OnItemSlotChanged.Remove(OnItemSlotChangedHandle);
	EquipmentManagerComponent->OnItemSlotChanged.Remove(OnEquipmentSlotChangedHandle);
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
	if (InventoryManagerComponent.IsValid()) {
		InventoryManagerComponent->ServerUseItem(TargetASC, MoveTemp(SlotHandle), AmountToUse);
	}
}

void UBackpackWidgetController::EquipItem(FInventoryItemSlotHandle SlotHandle) {
	if (EquipmentManagerComponent.IsValid()) {
		EquipmentManagerComponent->ServerMoveItem(MoveTemp(SlotHandle), EquipmentManagerComponent.Get(), 1);
	}
}

void UBackpackWidgetController::UnequipItem(FInventoryItemSlotHandle EquipmentSlotHandle) {
	if (EquipmentManagerComponent.IsValid()) {
		EquipmentManagerComponent->ServerMoveItem(MoveTemp(EquipmentSlotHandle), InventoryManagerComponent.Get(), 1);
	}
}

void UBackpackWidgetController::MoveItem(FInventoryItemSlotHandle FromSlotHandle,
                                         FInventoryItemSlotHandle ToSlotHandle,
                                         int32 AmountToMove) {
	if (InventoryManagerComponent.IsValid()) {
		InventoryManagerComponent->ServerMoveItemWithSlot(MoveTemp(FromSlotHandle), MoveTemp(ToSlotHandle), AmountToMove);
	}
}

void UBackpackWidgetController::OpenContextMenu(UUserWidget* UserWidget, FInventoryItemSlotHandle SlotHandle) {
	PreviousUserWidget = CurrentUserWidget;
	CurrentUserWidget = UserWidget;
	CurrentItemSlotHandle = SlotHandle;
	CurrentItemSlotInfo = UInventoryStatics::GetItemSlotInfo(SlotHandle);
	OnContextMenuChangedDelegate.Broadcast();
}

void UBackpackWidgetController::CloseContextMenu() {
	PreviousUserWidget = CurrentUserWidget;
	CurrentUserWidget = nullptr;
	CurrentItemSlotHandle = FInventoryItemSlotHandle::EmptyHandle;
	CurrentItemSlotInfo = FItemSlotInfo();
	OnContextMenuChangedDelegate.Broadcast();
}

bool UBackpackWidgetController::IsContextMenuOpened() {
	return CurrentUserWidget != nullptr;
}

FInventoryItemSlotHandle UBackpackWidgetController::GetEquipmentSlot(FGameplayTag EquipmentTag, int32 Index) {
	TArray<FInventoryItemSlotHandle> Result = EquipmentManagerComponent->GetItemSlotsByTag(EquipmentTag);
	if (Index < Result.Num()) 
		return Result[Index];
	return FInventoryItemSlotHandle::EmptyHandle;
}
