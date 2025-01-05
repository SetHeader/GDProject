// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/InventoryItemDefinition.h"

#include "Inventory/InventoryItemFragment.h"
#include "Net/UnrealNetwork.h"

UInventoryItemDefinition::UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
}

const UInventoryItemFragment* UInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UInventoryItemFragment> FragmentClass) const {
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

UInventoryItemFragment* UInventoryItemDefinition::
FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) {
	return const_cast<UInventoryItemFragment*>(
		static_cast<const UInventoryItemDefinition*>(this)->FindFragmentByClass(FragmentClass)
	);
}

void UInventoryItemDefinition::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const {
	ItemTags.ApplyTo(TagContainer);
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

void UInventoryItemDefinition::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UInventoryItemDefinition, ItemName);
	DOREPLIFETIME(UInventoryItemDefinition, ItemTags);
	DOREPLIFETIME(UInventoryItemDefinition, MaxStackCount);
	DOREPLIFETIME(UInventoryItemDefinition, ItemFragments);
}
