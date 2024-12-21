// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

#include "GDLog.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemFragment.h"

UInventoryItemFragment* UInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UInventoryItemFragment> FragmentClass) {
	if (FragmentClass == nullptr) {
		return nullptr;
	}

	if (ItemDefinition) {
		if (UInventoryItemFragment* Fragment = ItemDefinition->FindFragmentByClass(FragmentClass)) {
			return Fragment;
		}
	}
	
	return nullptr;
}

TArray<UInventoryItemFragment*> UInventoryItemInstance::GetFragments() {
	return ItemDefinition->GetFragments();
}

int32 UInventoryItemInstance::GetMaxStackCount() const {
	return ItemDefinition ? ItemDefinition->MaxStackCount : 1;
}

FText UInventoryItemInstance::GetItemName() const {
	return ItemDefinition ? ItemDefinition->ItemName : FText::FromString("null");
}

FGameplayTagContainer UInventoryItemInstance::GetItemTags() const {
	if (ItemDefinition) {
		FGameplayTagContainer Results;
		ItemDefinition->ItemTags.ApplyTo(Results);
		return Results;
	}
	return FGameplayTagContainer::EmptyContainer;
}

void UInventoryItemInstance::SetItemDifinition(TSubclassOf<UInventoryItemDefinition> InDefinition) {
	ItemDefinition = NewObject<UInventoryItemDefinition>(this, InDefinition);

	if (!ItemDefinition) {
		GDLOG_W(TEXT("UInventoryItemInstance"), TEXT("ItemDefinition is nullptr"));
		return;
	}
	
	for (auto& ItemFragment : ItemDefinition->GetFragments()) 
		ItemFragment->OnInstancedCreated(this);
}

void UInventoryItemInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const {
	if (ItemDefinition) {
		ItemDefinition->GetOwnedGameplayTags(TagContainer);
	} else {
		GDLOG_W(TEXT("UInventoryItemInstance"), TEXT("ItemDefinition is nullptr"));
	}
}

FString UInventoryItemInstance::ToString() const {
	if (ItemDefinition) 
		return ItemDefinition->GetName();
	
	return GetName();
}

bool UInventoryItemInstance::operator==(const UInventoryItemInstance& Other) const {
	return ItemDefinition && Other.ItemDefinition && *ItemDefinition == *Other.ItemDefinition;
}
