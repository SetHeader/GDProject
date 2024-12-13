// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryStatics.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GDLog.h"
#include "Interface/EquipmentManagerInferface.h"
#include "Interface/InventoryManagerInferface.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Fragment/ItemFragment_Available.h"
#include "Inventory/Fragment/ItemFragment_BaseInfo.h"

UInventoryManagerComponent* UInventoryStatics::GetInventoryManagerComponent(AActor* Actor) {
	if (IsValid(Actor) && Actor->Implements<UInventoryManagerInterface>()) {
		return Cast<IInventoryManagerInterface>(Actor)->GetInventoryManagerComponent();
	}
	return nullptr;
}

UEquipmentManagerComponent* UInventoryStatics::GetEquipmentManagerComponent(AActor* Actor) {
	if (IsValid(Actor) && Actor->Implements<UEquipmentManagerInterface>()) {
		return Cast<IEquipmentManagerInterface>(Actor)->GetEquipmentManagerComponent();
	}
	return nullptr;
}

UInventoryItemInstance* UInventoryStatics::GenerateItemInstance(TSubclassOf<UInventoryItemDefinition> ItemDefinition) {
	if (!ItemDefinition) {
		GDLOG(TEXT("InventorySubsystem"), TEXT("ItemDefinition Is Invalid"));
		return nullptr;
	}
	UInventoryItemInstance* ItemInstance = NewObject<UInventoryItemInstance>();
	ItemInstance->SetItemDifinition(ItemDefinition);
	return ItemInstance;
}

FItemSlotInfo UInventoryStatics::GetItemSlotInfo(FInventoryItemSlotHandle SlotHandle) {
	check(SlotHandle.IsValid());
	UInventoryManagerComponent* InventoryManagerComponent = SlotHandle.GetOwner();
	check(IsValid(InventoryManagerComponent));

	FItemSlotInfo SlotInfo;

	FInventoryItemSlot ItemSlot = InventoryManagerComponent->GetItemSlot(SlotHandle);

	SlotInfo.SlotTags = ItemSlot.SlotTags;
	if (!ItemSlot.ItemInstance)
		return SlotInfo;

	SlotInfo.ContainItem = true;
	SlotInfo.StackCount = ItemSlot.StackCount;
	
	SlotInfo.ItemName = ItemSlot.ItemInstance->GetItemName();
	SlotInfo.ItemTags = ItemSlot.ItemInstance->GetItemTags();
	
	if (UItemFragment_BaseInfo* BaseInfoFragment = ItemSlot.ItemInstance->FindFragmentByClass<UItemFragment_BaseInfo>()) {
        SlotInfo.Icon = BaseInfoFragment->Icon;
        SlotInfo.Description = BaseInfoFragment->Description;
	}
	
	return SlotInfo;
}

void UInventoryStatics::UseItem(UAbilitySystemComponent* TargetASC,
                                FInventoryItemSlotHandle SlotHandle,
                                int32 AmountToUse) {
	check(IsValid(TargetASC));
	check(SlotHandle.IsValid());
	check(AmountToUse > 0);
	UInventoryManagerComponent* InventoryManagerComponent = SlotHandle.GetOwner();
	check(IsValid(InventoryManagerComponent));
	
	auto ItemSlot = InventoryManagerComponent->GetItemSlot(SlotHandle);
	if (!ItemSlot.IsValid())
		return;
	
	FInventoryRemovedItemInfo RemovedItemInfo;
	InventoryManagerComponent->RemoveItemFromSlot(SlotHandle, AmountToUse, RemovedItemInfo);
	if (RemovedItemInfo.ItemInstance) {
		auto* AvailableFragment = RemovedItemInfo.ItemInstance->FindFragmentByClass<UItemFragment_Available>();
		if (AvailableFragment) {
			FUseItemParams Params;
			Params.TargetASC = TargetASC;
			for (int i = 0; i < RemovedItemInfo.Count; i++) {
				AvailableFragment->UseItem(Params);
			}
		}
	}
}

void UInventoryStatics::MoveItem(FInventoryItemSlotHandle FromSlotHandle,
	FInventoryItemSlotHandle ToSlotHandle,
	int32 AmountToMove) {
	check(FromSlotHandle.IsValid());
	check(ToSlotHandle.IsValid());
	check(AmountToMove > 0);

	UInventoryManagerComponent* FromInventoryComponent = FromSlotHandle.GetOwner();
	UInventoryManagerComponent* ToInventoryComponent = ToSlotHandle.GetOwner();
	check(IsValid(FromInventoryComponent));
	check(IsValid(ToInventoryComponent));

	FInventoryItemSlot FromSlot = FromInventoryComponent->GetItemSlot(FromSlotHandle);
	FInventoryItemSlot ToSlot = ToInventoryComponent->GetItemSlot(ToSlotHandle);

	if (!FromSlot.ItemInstance)
		return;

	// 目标插槽没物品才能移动
	if (!ToSlot.ItemInstance) {
		FInventoryRemovedItemInfo RemovedItemInfo;
		FromInventoryComponent->RemoveItemFromSlot(FromSlotHandle, AmountToMove, RemovedItemInfo);
		if (RemovedItemInfo.ItemInstance) 
			ToInventoryComponent->AddItemToSlot(RemovedItemInfo.ItemInstance, ToSlotHandle, RemovedItemInfo.Count);
	}
}

void UInventoryStatics::SwapItem(FInventoryItemSlotHandle FromSlotHandle, FInventoryItemSlotHandle ToSlotHandle) {
	check(FromSlotHandle.IsValid());
	check(ToSlotHandle.IsValid());
	
	UInventoryManagerComponent* FromInventoryComponent = FromSlotHandle.GetOwner();
	UInventoryManagerComponent* ToInventoryComponent = ToSlotHandle.GetOwner();
	check(IsValid(FromInventoryComponent));
	check(IsValid(ToInventoryComponent));

	FInventoryItemSlot FromSlot = FromInventoryComponent->GetItemSlot(FromSlotHandle);
	FInventoryItemSlot ToSlot = ToInventoryComponent->GetItemSlot(ToSlotHandle);

	// 两个手柄都没有物品就不处理
	if (!FromSlot.ItemInstance && !ToSlot.ItemInstance)
		return;

	// 目标插槽没物品才能移动
	FInventoryRemovedItemInfo FromRemovedItemInfo;
	FInventoryRemovedItemInfo ToRemovedItemInfo;
	FromInventoryComponent->RemoveAllItemFromSlot(FromSlotHandle, FromRemovedItemInfo);
	ToInventoryComponent->RemoveAllItemFromSlot(ToSlotHandle, ToRemovedItemInfo);

	FromInventoryComponent->AddItemToSlot(FromRemovedItemInfo.ItemInstance, FromSlotHandle, FromRemovedItemInfo.Count);
	ToInventoryComponent->AddItemToSlot(ToRemovedItemInfo.ItemInstance, ToSlotHandle, ToRemovedItemInfo.Count);
}


FUseItemParams UInventoryStatics::MakeUseItemParams(AActor* TargetActor) {
	FUseItemParams Params;
	if (IsValid(TargetActor)) {
		Params.TargetActor = TargetActor;
		Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	}
	return Params;
}

FUseItemParams UInventoryStatics::MakeUseItemParamsWithSourceActor(AActor* SourceActor, AActor* TargetActor) {
	FUseItemParams Params;
	if (IsValid(SourceActor)) {
		Params.SourceActor = SourceActor;
		Params.SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(SourceActor);
	}
	if (IsValid(TargetActor)) {
		Params.TargetActor = TargetActor;
		Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	}
	return Params;
}

FString UInventoryStatics::StringifyItemSlot(const FInventoryItemSlot& ItemSlot) {
	return ItemSlot.ToString();
}
