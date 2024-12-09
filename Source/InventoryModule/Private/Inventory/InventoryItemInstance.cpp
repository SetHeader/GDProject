// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

#include "GDLog.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemFragment.h"
#include "Inventory/Fragment/ItemFragment_Equipment.h"
#include "Inventory/Fragment/ItemFragment_EquipmentInstance.h"

const UInventoryItemFragment* UInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UInventoryItemFragment> FragmentClass) const {
	if (FragmentClass == nullptr) {
		return nullptr;
	}

	if (ItemDefinition) {
		if (const UInventoryItemFragment* Fragment = ItemDefinition->FindFragmentByClass(FragmentClass)) {
			return Fragment;
		}
	}
	
	for (const auto& Fragment : DynamicFragments) {
		if (Fragment && Fragment.IsA(FragmentClass)) {
			return Fragment;
		}
	}

	return nullptr;
}

void UInventoryItemInstance::SetItemDifinition(TObjectPtr<UInventoryItemDefinition> InDefinition) {
	ItemDefinition = InDefinition;

	DynamicFragments.Reset();
	// TODO 让父类去依赖子类，不好，得优化一下。
	if (const auto* EquipmentFragment = ItemDefinition->FindFragmentByClass<UItemFragment_Equipment>()) {
		TSubclassOf<UItemPerk> Perk = EquipmentFragment->EquipmentPerk.LoadSynchronous();
		if (Perk) {
			UItemFragment_EquipmentInstance* EquipmentFragmentInstance = NewObject<UItemFragment_EquipmentInstance>(this);
			EquipmentFragmentInstance->PerkContainer = Perk.GetDefaultObject()->GenerateItemPerk();
			DynamicFragments.Add(EquipmentFragmentInstance);
		}
	}
}

void UInventoryItemInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const {
	if (ItemDefinition) {
		ItemDefinition->GetOwnedGameplayTags(TagContainer);
	} else {
		GDLOG_W(TEXT("UInventoryItemInstance"), TEXT("ItemDefinition is nullptr"));
	}
}
