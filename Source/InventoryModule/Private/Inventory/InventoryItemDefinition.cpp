// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemFragment.h"

UInventoryItemDefinition::UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
}

const UInventoryItemFragment* UInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UInventoryItemFragment> FragmentClass) {
	if (FragmentClass == nullptr) {
		return nullptr;
	}
	
	for (const auto& Fragment : Fragments) {
		if (Fragment && Fragment.IsA(FragmentClass)) {
			return Fragment;
		}
	}
	
	return nullptr;
}

void UInventoryItemDefinition::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const {
	TagContainer.AppendTags(ItemTags.CombinedTags);
}