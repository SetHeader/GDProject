// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemDefinition.h"

#include "Algo/Compare.h"
#include "Inventory/InventoryItemFragment.h"

UInventoryItemDefinition::UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
}

UInventoryItemFragment* UInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UInventoryItemFragment> FragmentClass) {
	if (FragmentClass == nullptr) {
		return nullptr;
	}
	
	for (const auto& Fragment : ItemFragments) {
		if (Fragment && Fragment.IsA(FragmentClass)) {
			return Fragment;
		}
	}
	
	return nullptr;
}

void UInventoryItemDefinition::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const {
	TagContainer.AppendTags(ItemTags.CombinedTags);
}

bool UInventoryItemDefinition::operator==(const UInventoryItemDefinition& Other) const {
	
	bool Result = ItemName.EqualTo(Other.ItemName) && MaxStackCount == Other.MaxStackCount
		&& ItemTags == Other.ItemTags;
	if (!Result)
		return false;

	if (ItemFragments.Num() != Other.ItemFragments.Num())
		return false;
	
	for (int i = 0; i < ItemFragments.Num(); ++i) {
		if (*ItemFragments[i] != *Other.ItemFragments[i]) {
			return false;
		}
	}
	return true;
}

bool UInventoryItemDefinition::operator!=(const UInventoryItemDefinition& Other) const {
	return !operator==(Other);
}
