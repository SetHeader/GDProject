#include "Inventory/InventoryItemSlot.h"

#include "Inventory/InventoryManagerComponent.h"

const FInventoryItemSlotHandle FInventoryItemSlotHandle::EmptyHandle;
const FInventoryItemSlot FInventoryItemSlot::EmptySlot;

FInventoryItemSlotHandle::FInventoryItemSlotHandle(const FInventoryItemSlot& Slot): Owner(Slot.GetOwner()), SlotId(Slot.SlotId) {

}
 
UInventoryItemInstance* FInventoryItemSlotHandle::GetItemInstance() const {
	return GetSlot().ItemInstance;
}

int32 FInventoryItemSlotHandle::GetStackCount() const {
	return GetSlot().StackCount;
}

FGameplayTagContainer FInventoryItemSlotHandle::GetSlotTags() const {
	return GetSlot().SlotTags;
}

FInventoryItemSlot FInventoryItemSlotHandle::GetSlot() const {
	if (IsValid()) {
		return GetOwner()->GetItemSlot(FInventoryItemSlotHandle(*this));
	}
	return FInventoryItemSlot::EmptySlot;
}

UInventoryManagerComponent* FInventoryItemSlotHandle::GetOwner() const {
	return Owner.Get();
}

int32 FInventoryItemSlot::GetMaxStackCount() const {
	return ItemInstance ? FMath::Min(MaxStackCount, ItemInstance->GetMaxStackCount()) : MaxStackCount;
}

int32 FInventoryItemSlot::GetRemainingStackCount() const {
	return FMath::Max(0, GetMaxStackCount() - StackCount);
}
