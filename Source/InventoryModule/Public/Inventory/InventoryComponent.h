// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InventoryItemSlot.h"
#include "UObject/Object.h"
#include "InventoryComponent.generated.h"

class UAbilitySystemComponent;
class UInventoryComponent;
struct FInventoryItemSlot;

USTRUCT()
struct INVENTORYMODULE_API FInventoryItemSlotArray {
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventoryItemSlot> Slots;

	UPROPERTY()
	TObjectPtr<UInventoryComponent> Owner;
};

USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FInventoryItemSlotHandle {
	GENERATED_BODY()

	FInventoryItemSlotHandle() {}

	FInventoryItemSlotHandle(int32 InSlotId, UInventoryComponent* InParentInventory)
		: SlotId(InSlotId), ParentInventory(InParentInventory) {}

	FInventoryItemSlotHandle(FGameplayTag InTag, UInventoryComponent* InParentInventory)
		: SlotTags(InTag), ParentInventory(InParentInventory) {}

	FInventoryItemSlotHandle(const FInventoryItemSlotHandle& Other)
		: SlotId(Other.SlotId), SlotTags(Other.SlotTags), ParentInventory(Other.ParentInventory) {}

	FInventoryItemSlotHandle(FInventoryItemSlotHandle&& Other)
		: SlotId(MoveTemp(Other.SlotId)), SlotTags(MoveTemp(Other.SlotTags)), ParentInventory(MoveTemp(Other.ParentInventory)) {}

	FInventoryItemSlotHandle(const FInventoryItemSlot& FromSlot, UInventoryComponent* InParentInventory)
		: SlotId(FromSlot.SlotId), SlotTags(FromSlot.SlotTags), ParentInventory(InParentInventory) {}

	FInventoryItemSlotHandle& operator=(const FInventoryItemSlotHandle&) = default;
	FInventoryItemSlotHandle& operator=(FInventoryItemSlotHandle&&) = default;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 SlotId = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	FGameplayTagContainer SlotTags = FGameplayTagContainer();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	TObjectPtr<UInventoryComponent> ParentInventory = nullptr;

	bool operator==(const FInventoryItemSlotHandle& Other) const {
		return SlotId == Other.SlotId && SlotTags == Other.SlotTags && ParentInventory == Other.ParentInventory;
	}

	bool operator!=(const FInventoryItemSlotHandle& Other) const {
		return !operator==(Other);
	}

	bool IsValid() const {
		return SlotId > 0;
	}
};

inline bool operator==(const FInventoryItemSlotHandle& lhs, const FInventoryItemSlot& rhs) {
	return lhs.SlotId == rhs.SlotId && lhs.SlotTags == rhs.SlotTags;
}

inline bool operator!=(const FInventoryItemSlotHandle& lhs, const FInventoryItemSlot& rhs) {
	return !operator==(lhs, rhs);
}

inline bool operator==(const FInventoryItemSlot& lhs, const FInventoryItemSlotHandle& rhs) {
	return operator==(rhs, lhs);
}

inline bool operator!=(const FInventoryItemSlot& lhs, const FInventoryItemSlotHandle& rhs) {
	return operator!=(rhs, lhs);
}

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdatedSignature, UInventoryComponent* /*this*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FOnItemSlotUpdatedSignature
	, UInventoryComponent* /*this*/, const FInventoryItemSlotHandle& /*SlotHandle*/, UInventoryItemInstance* /*UpdatedItem*/, UInventoryItemInstance* /*PreviousItem*/);

/**
 * 物品存储与管理组件
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = Inventory, meta = (BlueprintSpawnableComponent))
class INVENTORYMODULE_API UInventoryComponent : public UActorComponent {
	GENERATED_BODY()

public:
	bool IsValidItemSlot(const FInventoryItemSlotHandle& Handle) const;

	FInventoryItemSlot& GetItemSlot(const FInventoryItemSlotHandle& Handle);

	// 把物品放到第一个空的插槽中
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool LootItem(UInventoryItemInstance* Item);
	// 把物品放到指定插槽中
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool PlaceItemIntoSlot(UInventoryItemInstance* Item, const FInventoryItemSlotHandle& SlotHandle);
	// 判断指定插槽能否放入指定物品
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual bool AcceptItem(UInventoryItemInstance* Item, const FInventoryItemSlotHandle& SlotHandle);

protected:
	// 创建插槽
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void CreateInventorySlot(const FGameplayTagContainer& SlotTags, const FInventoryItemSlotFilter& Filter);
	// 删除已有插槽
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual void RemoveInventorySlot(const FInventoryItemSlotHandle& Handle);
	// 当背包系统插槽发生变化时调用，如新增或移除插槽
	void PostInventoryUpdate();
	// 复制插槽所有handle到外部
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void PopulateSlotReferenceArray(TArray<FInventoryItemSlotHandle>& Handles);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemFromInventory(const FInventoryItemSlotHandle& SlotHandle);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveAllItemFromInventory(TArray<UInventoryItemInstance*>& OutItemsRemoved);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual TArray<FInventoryItemSlotHandle> GetAllSlotHandles();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual UInventoryItemInstance* GetItemInstanceInSlot(const FInventoryItemSlotHandle& SlotHandle);
protected:
	UPROPERTY()
	FInventoryItemSlotArray SlotArray;

	TArray<FInventoryItemSlotHandle> SlotHandles;

	uint32 IdCounter = 0;

	FOnInventoryUpdatedSignature OnInventoryUpdatedDelegate;
	FOnItemSlotUpdatedSignature OnItemSlotUpdatedDelegate;
};
