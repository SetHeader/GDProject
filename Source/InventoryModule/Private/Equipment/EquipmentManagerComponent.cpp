// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/EquipmentManagerComponent.h"

#include "AbilitySystemComponent.h"
#include "GDLog.h"
#include "IDetailTreeNode.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Equippable.h"

void UEquipmentManagerComponent::InitializeComponent() {
	Super::InitializeComponent();
	GDLOG(TEXT("InventoryComponent_Equipment"), TEXT("InitializeComponent"));
	for (FInventoryItemSlot& Slot : Slots) {
		Slot.MaxStackCount = 1;
		Slot.StackCount = FMath::Clamp(Slot.StackCount, 0, Slot.MaxStackCount);
	}

	OnItemSlotUpdatedDelegate.AddUObject(this, &UEquipmentManagerComponent::OnEquipSlotUpdate);
}

void UEquipmentManagerComponent::UninitializeComponent() {
	GDLOG(TEXT("InventoryComponent_Equipment"), TEXT("UninitializeComponent"));
	OnItemSlotUpdatedDelegate.RemoveAll(this);
	Super::UninitializeComponent();
}

FInventoryItemSlotHandle UEquipmentManagerComponent::CreateInventorySlot(const FGameplayTagContainer& SlotTags) {
	return Super::CreateInventorySlotWithMaxStackCount(SlotTags, 1);
}

FInventoryItemSlotHandle UEquipmentManagerComponent::CreateInventorySlotWithMaxStackCount(
	const FGameplayTagContainer& SlotTags,
	int32 MaxStackCount) {
	if (MaxStackCount != 1) {
		GDLOG_W(TEXT("UEquipmentManagerComponent"), TEXT("MaxStackCount必须等于1"));
		return FInventoryItemSlotHandle::EmptyHandle;
	}
	return Super::CreateInventorySlotWithMaxStackCount(SlotTags, MaxStackCount);
}

FInventoryItemSlotHandle UEquipmentManagerComponent::EquipItem(UInventoryItemInstance* Item) {
	if (!IsValid(Item)) 
		return FInventoryItemSlotHandle::EmptyHandle;

	FGameplayTagContainer ItemTags;
	Item->GetOwnedGameplayTags(ItemTags);
	TArray<FInventoryItemSlot*> TargetSlots = GetItemSlotsByTags(ItemTags);
	if (TargetSlots.IsEmpty())
		return FInventoryItemSlotHandle::EmptyHandle;
	
	// 优先装备到可用的空插槽上
	for (auto* TargetSlot : TargetSlots) {
		if (!TargetSlot->ItemInstance && Super::AddItemToSlot(Item, *TargetSlot)) 
			return *TargetSlot;
	}

	// 没办法才装备到非空插槽上，替换掉原有的物品
	if (EquipItemToSlot(Item, FInventoryItemSlotHandle(*TargetSlots[0])))
		return *TargetSlots[0];
	return FInventoryItemSlotHandle::EmptyHandle;
}

bool UEquipmentManagerComponent::EquipItemToSlot(UInventoryItemInstance* Item,
	const FInventoryItemSlotHandle& SlotHandle) {
	return Super::AddItemToSlot(Item, SlotHandle) > 0;
}

bool UEquipmentManagerComponent::UnequipItem(FInventoryItemSlotHandle SlotHandle) {
	FInventoryRemovedItemInfo Info;
	RemoveItemFromSlot(SlotHandle, 1, Info);
	return Info.ItemInstance != nullptr;
}

int32 UEquipmentManagerComponent::AddItem(UInventoryItemInstance* Item, int32 Count) {
	return EquipItem(Item).IsValid() ? 1 : 0;
}

TArray<FInventoryItemSlot> UEquipmentManagerComponent::BP_GetItemSlotsByTag(const FGameplayTag& Tag) {
	return BP_GetItemSlotsByTags(FGameplayTagContainer(Tag));
}

TArray<FInventoryItemSlot> UEquipmentManagerComponent::BP_GetItemSlotsByTags(const FGameplayTagContainer& Tags) {
	TArray<FInventoryItemSlot*> Arr = GetItemSlotsByTags(Tags);
	TArray<FInventoryItemSlot> Result;
	Result.SetNum(Arr.Num());
	Algo::Transform(Arr, Result, [](FInventoryItemSlot* Item) {
		return *Item;	
	});
	return Result;
}

void UEquipmentManagerComponent::OnEquipSlotUpdate(const FItemSlotChangedData& ChangedData) {
	if (ChangedData.PreviousItem)
		Internal_HandleItemUnequipped(ChangedData.SlotHandle);
	
	if (ChangedData.CurrentItem) 
		Internal_HandleItemEquipped(ChangedData.SlotHandle, ChangedData.CurrentItem);
}

void UEquipmentManagerComponent::Internal_HandleItemEquipped(const FInventoryItemSlotHandle& SlotHandle,
	UInventoryItemInstance* ItemInstance) {
	check(SlotHandle.IsValid())
	check(ItemInstance);
	
	const UItemFragment_Equippable* EquipmentFragment = ItemInstance->FindFragmentByClass<UItemFragment_Equippable>();
	if (!EquipmentFragment)
		return;

	if (UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent()) {
		FInventoryItemInfoEntry Entry(SlotHandle);
		EquipmentFragment->GiveToAbilitySystem(ASC, &Entry.AbilitySet_GrantHandles, ItemInstance);
		EquipmentInfos.Add(Entry);
	}
}

void UEquipmentManagerComponent::Internal_HandleItemUnequipped(const FInventoryItemSlotHandle& SlotHandle) {
	check(SlotHandle.IsValid());
	
	FInventoryItemInfoEntry* Entry = EquipmentInfos.FindByPredicate([SlotHandle](const FInventoryItemInfoEntry& InEntry) {
		return SlotHandle == InEntry.SlotHandle;
	});
	if (!Entry)
		return;

	if (UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent()) 
		Entry->AbilitySet_GrantHandles.TakeFromAbilitySystem(ASC);

	EquipmentInfos.RemoveAll([SlotHandle](const FInventoryItemInfoEntry& InEntry) {
		return SlotHandle == InEntry.SlotHandle;
	});
}

TArray<FInventoryItemSlot*> UEquipmentManagerComponent::GetItemSlotsByTag(const FGameplayTag& Tag) {
	return GetItemSlotsByTags(FGameplayTagContainer(Tag));
}

TArray<FInventoryItemSlot*> UEquipmentManagerComponent::GetItemSlotsByTags(const FGameplayTagContainer& Tags) {
	TArray<FInventoryItemSlot*> Result;
	for (FInventoryItemSlot& Slot : Slots) {
		if (Tags.HasAny(Slot.SlotTags))
			Result.Add(&Slot);
	}

	return Result;
}

bool UEquipmentManagerComponent::IsItemAcceptable(const UInventoryItemInstance* Item,
	const FInventoryItemSlot& Slot) const {
	if (!Super::IsItemAcceptable(Item, Slot))
		return false;
	
	FGameplayTagContainer ItemTags;
	Item->GetOwnedGameplayTags(ItemTags);
	
	return ItemTags.HasAny(Slot.SlotTags);
}
