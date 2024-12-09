#pragma once
#include "GameplayTagContainer.h"

#include "InventoryItemSlot.generated.h"

class UInventoryComponent;
class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FInventoryItemSlotFilter {
	GENERATED_BODY()

	FInventoryItemSlotFilter(){}
	int filter = 0;
};

/**
 * 物品插槽
 */
USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FInventoryItemSlot {
	GENERATED_BODY()

	FInventoryItemSlot() {}
	// 当前插槽包含的物品
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "Inventory")
	UInventoryItemInstance* ItemInstance = nullptr;
	
	UPROPERTY()
	FInventoryItemSlotFilter ItemSlotFilter = FInventoryItemSlotFilter();

	// 插槽id
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Inventory")
	int32 SlotId = 0;
	// 插槽tag，可以指定是否有交易保护、掉落保护等
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Inventory")
	FGameplayTagContainer SlotTags = FGameplayTagContainer::EmptyContainer;

	bool operator==(const FInventoryItemSlot& Other) const {
		return SlotId == Other.SlotId && ItemInstance == Other.ItemInstance;
	}

	bool operator!=(const FInventoryItemSlot& Other) const {
		return !operator==(Other);
	}

	static const FInventoryItemSlot EmptySlot;

protected:
	TObjectPtr<UInventoryComponent> Owner;
	friend class UInventoryComponent;
};