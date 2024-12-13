#pragma once

#include "GameplayTagContainer.h"
#include "InventoryItemInstance.h"
#include "Templates\SharedPointer.h"
#include "InventoryItemSlot.generated.h"

struct FInventoryItemSlotHandle;
struct FInventoryItemSlot;
class UInventoryManagerComponent;
class UInventoryItemInstance;

USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FInventoryItemSlotHandle {
	GENERATED_BODY()

public:
	static const FInventoryItemSlotHandle EmptyHandle;

public:
	FInventoryItemSlotHandle() {}
	FInventoryItemSlotHandle(const FInventoryItemSlotHandle& Other) = default;
	FInventoryItemSlotHandle(FInventoryItemSlotHandle&& Other) = default;
	FInventoryItemSlotHandle(const FInventoryItemSlot& Slot);

	FInventoryItemSlotHandle& operator=(const FInventoryItemSlotHandle&) = default;
	FInventoryItemSlotHandle& operator=(FInventoryItemSlotHandle&&) = default;

	UInventoryItemInstance* GetItemInstance() const;

	int32 GetStackCount() const;

	FGameplayTagContainer GetSlotTags() const;

	FInventoryItemSlot GetSlot() const;

	UInventoryManagerComponent* GetOwner() const;

	int32 GetSlotId() const { return SlotId; }
	
	bool operator==(const FInventoryItemSlotHandle& Other) const {
		return Owner == Other.Owner && SlotId > 0;
	}

	bool operator!=(const FInventoryItemSlotHandle& Other) const {
		return !operator==(Other);
	}

	bool IsValid() const {
		return Owner.IsValid() && SlotId > 0;
	}
private:
	UPROPERTY()
	TWeakObjectPtr<UInventoryManagerComponent> Owner = nullptr;
	int32 SlotId = 0;
};

/**
 * 物品插槽
 */
USTRUCT(BlueprintType, Blueprintable)
struct INVENTORYMODULE_API FInventoryItemSlot {
	GENERATED_BODY()

	friend class UInventoryManagerComponent;
	
	// 当前插槽包含的物品
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	UInventoryItemInstance* ItemInstance = nullptr;

	// 插槽id，>0 才有效，<=0为无效
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 SlotId = 0;

	// 插槽tag，可以指定插槽类型，如武器、防具等
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FGameplayTagContainer SlotTags = FGameplayTagContainer::EmptyContainer;

	// 物品堆叠数量
	UPROPERTY(BlueprintReadOnly, Category = "Inventory", meta = (ClampMin = 0))
	int32 StackCount = 0;

	// 物品最大堆叠数量，<=0表示不能存放物品
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (ClampMin = 0))
	int32 MaxStackCount = INT32_MAX;

public:
	FInventoryItemSlot() {}

	bool operator==(const FInventoryItemSlot& Other) const {
		return SlotId == Other.SlotId && Owner == Other.Owner;
	}

	bool operator!=(const FInventoryItemSlot& Other) const {
		return !operator==(Other);
	}

	bool IsValid() const { return SlotId > 0 && Owner.IsValid(); }

	UInventoryManagerComponent* GetOwner() const { return Owner.Get(); }
	// 最大堆叠数量取决于 Slot的最大堆叠数量 与 Item内定义的最大堆叠数量。
	// 取两者中较小的那个。
	int32 GetMaxStackCount() const;
	// 剩余堆叠数量。最大堆叠数量 - 当前堆叠数量。
	int32 GetRemainingStackCount() const;

	FString ToString() const {
		FString ItemString = ItemInstance ? ItemInstance->ToString() : TEXT("nullptr");
		return FString::Printf(TEXT("ItemSlot{id:%d tags:%ls item:%ls stack:%d/%d}"), SlotId, *SlotTags.ToString(), *ItemString, StackCount, MaxStackCount);
	}
public:
	static const FInventoryItemSlot EmptySlot;
	
private:
	UPROPERTY()
	TWeakObjectPtr<UInventoryManagerComponent> Owner;
};