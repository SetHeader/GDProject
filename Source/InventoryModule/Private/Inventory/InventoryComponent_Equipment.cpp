// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent_Equipment.h"

#include "AbilitySystemComponent.h"
#include "GDLog.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Equipment.h"

UInventoryComponent_Equipment::UInventoryComponent_Equipment(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	bWantsInitializeComponent = true;
}

void UInventoryComponent_Equipment::InitializeComponent() {
	Super::InitializeComponent();
	GDLOG(TEXT("InventoryComponent_Equipment"), TEXT("InitializeComponent"));
	OnItemSlotUpdatedDelegate.AddUObject(this, &UInventoryComponent_Equipment::OnEquipSlotUpdate);
}

void UInventoryComponent_Equipment::UninitializeComponent() {
	Super::UninitializeComponent();
	GDLOG(TEXT("InventoryComponent_Equipment"), TEXT("UninitializeComponent"));
}

void UInventoryComponent_Equipment::OnEquipSlotUpdate(UInventoryComponent* InventoryComponent,
                                                      const FInventoryItemSlotHandle& SlotHandle,
                                                      UInventoryItemInstance* ItemInstance,
                                                      UInventoryItemInstance* PreviousItemInstance) {
	if (!SlotHandle.IsValid()) 
		return;
	
	if (IsValid(PreviousItemInstance)) 
		MakeItemUnequipped_Internal(SlotHandle, PreviousItemInstance);
	
	if (IsValid(ItemInstance)) 
		MakeItemEquipped_Internal(SlotHandle, ItemInstance);
}

bool UInventoryComponent_Equipment::MakeItemEquipped_Internal(const FInventoryItemSlotHandle& SlotHandle) {
	if (!SlotHandle.IsValid())
		return false;

	return false;
}

bool UInventoryComponent_Equipment::MakeItemEquipped_Internal(const FInventoryItemSlotHandle& SlotHandle,
	UInventoryItemInstance* ItemInstance) {
	if (!SlotHandle.IsValid())
		return false;

	const UItemFragment_Equipment* EquipmentFragment = Cast<UItemFragment_Equipment>(ItemInstance->FindFragmentByClass(UItemFragment_Equipment::StaticClass()));
	if (!EquipmentFragment)
		return false;

	if (UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent()) {
		FInventoryItemInfoEntry Entry(SlotHandle);
		EquipmentFragment->GiveToAbilitySystem(ASC, &Entry.AbilitySet_GrantHandles, ItemInstance);
		EquipmentInfos.Add(Entry);
	}

	return true;
}

bool UInventoryComponent_Equipment::MakeItemUnequipped_Internal(const FInventoryItemSlotHandle& SlotHandle) {
	return false;
}

bool UInventoryComponent_Equipment::MakeItemUnequipped_Internal(const FInventoryItemSlotHandle& SlotHandle,
	UInventoryItemInstance* ItemInstance) {

	if (!IsValid(ItemInstance))
		return false;

	FInventoryItemInfoEntry* Entry = EquipmentInfos.FindByPredicate([SlotHandle](const FInventoryItemInfoEntry& InEntry) {
		return SlotHandle == InEntry.SlotHandle;
	});
	if (!Entry)
		return false;

	if (UAbilitySystemComponent* ASC = GetOwnerAbilitySystemComponent()) 
		Entry->AbilitySet_GrantHandles.TakeFromAbilitySystem(ASC);

	if (SlotHandle.IsValid())
		EquipmentInfos.RemoveAll([SlotHandle](const FInventoryItemInfoEntry& InEntry) {
			return SlotHandle == InEntry.SlotHandle;
		});
	
	return true;
}
