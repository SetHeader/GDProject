// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryItemInstance.h"

#include "GDProject/GDProject.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemFragment.h"
#include "Net/UnrealNetwork.h"

const UInventoryItemFragment* UInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UInventoryItemFragment> FragmentClass) const {
	if (FragmentClass == nullptr) {
		return nullptr;
	}

	if (GetItemDefinition()) {
		if (const UInventoryItemFragment* Fragment = GetItemDefinition()->FindFragmentByClass(FragmentClass)) {
			return Fragment;
		}
	}
	
	return nullptr;
}

TArray<UInventoryItemFragment*> UInventoryItemInstance::GetFragments() const {
	TArray<UInventoryItemFragment*> Fragments;
	for (auto& Fragment : GetItemDefinition()->GetFragments()) {
		Fragments.Add(Fragment);
	}

	return Fragments;
}

const UInventoryItemDefinition* UInventoryItemInstance::GetItemDefinition() const {
	if (ItemDefinition)
		return ItemDefinition->GetDefaultObject<UInventoryItemDefinition>();
	return nullptr;
}

int32 UInventoryItemInstance::GetMaxStackCount() const {
	return GetItemDefinition() ? GetItemDefinition()->MaxStackCount : 1;
}

FText UInventoryItemInstance::GetItemName() const {
	return GetItemDefinition() ? GetItemDefinition()->ItemName : FText::FromString("null");
}

FGameplayTagContainer UInventoryItemInstance::GetItemTags() const {
	if (GetItemDefinition()) {
		FGameplayTagContainer Results;
		GetItemDefinition()->ItemTags.ApplyTo(Results);
		return Results;
	}
	return FGameplayTagContainer::EmptyContainer;
}

void UInventoryItemInstance::SetItemDifinition(TSubclassOf<UInventoryItemDefinition> InDefinition) {
	// 暂时不需要动态的实例，用CDO就行
	// ItemDefinition = NewObject<UInventoryItemDefinition>(this, InDefinition);
	//
	// if (!ItemDefinition) {
	// 	GDLOG_W(TEXT("UInventoryItemInstance"), TEXT("ItemDefinition is nullptr"));
	// 	return;
	// }
	//
	// for (auto& ItemFragment : ItemDefinition->GetFragments()) 
	// 	ItemFragment->OnInstancedCreated(this);
	ItemDefinition = InDefinition;
}

void UInventoryItemInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const {
	if (ItemDefinition) {
		ItemDefinition->GetDefaultObject<UInventoryItemDefinition>()->GetOwnedGameplayTags(TagContainer);
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

void UInventoryItemInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const {
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryItemInstance, ItemDefinition);
}
