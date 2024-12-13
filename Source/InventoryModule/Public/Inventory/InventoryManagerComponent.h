// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InventoryItemSlot.h"
#include "UObject/Object.h"
#include "InventoryManagerComponent.generated.h"

class UAbilitySystemComponent;
class UInventoryManagerComponent;
struct FInventoryItemSlot;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdatedSignature, UInventoryManagerComponent* /*this*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemSlotUpdatedSignature, const FItemSlotChangedData& /*ChangedData*/);

UENUM(BlueprintType)
enum class EItemSlotOperation : uint8 {
	NONE,
	ADD,	// 新增：之前的物品为空，或之前的物品和现在物品相同，只做堆叠。
	REMOVE,	// 移除：之前的物品不为空，减少物品堆叠，现在的物品可能为空（如堆叠为0即完全移除）
	REPLACE	// 替换：之前的物品和现在的物品不同，且都不为空
};

USTRUCT(BlueprintType)
struct FItemSlotChangedData {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryManagerComponent> InventoryComponent = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	EItemSlotOperation Operation = EItemSlotOperation::NONE;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	FInventoryItemSlotHandle SlotHandle = FInventoryItemSlotHandle();
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryItemInstance>	CurrentItem = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 CurrentStackCount = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryItemInstance> PreviousItem = nullptr;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 PreviousStackCount = 0;
};

/**
 * 被移除的物品信息
 */
USTRUCT(BlueprintType)
struct FInventoryRemovedItemInfo {
	GENERATED_BODY()
	
	// 当前插槽包含的物品
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	UInventoryItemInstance* ItemInstance = nullptr;
	
	// 物品堆叠数量
	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	int32 Count = 0;
};

/**
 * 物品存储与管理组件
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = Inventory, meta = (BlueprintSpawnableComponent))
class INVENTORYMODULE_API UInventoryManagerComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UInventoryManagerComponent(const FObjectInitializer& ObjectInitializer);
	
	virtual void InitializeComponent() override;
	
	// 创建插槽
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual FInventoryItemSlotHandle CreateInventorySlot(const FGameplayTagContainer& SlotTags);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual FInventoryItemSlotHandle CreateInventorySlotWithMaxStackCount(const FGameplayTagContainer& SlotTags, int32 MaxStackCount);
	
	// 删除已有插槽
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveInventorySlot(const FInventoryItemSlotHandle& Handle);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsValidItemSlot(const FInventoryItemSlotHandle& Handle) const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FInventoryItemSlot GetItemSlot(const FInventoryItemSlotHandle& Handle);
	
	// 把物品放到空插槽中。可能会放到多个插槽中。且可能只放入一部分的物品。
	// @return 放入物品的数量。
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual int32 AddItem(UInventoryItemInstance* Item, int32 Count = 1);
	
	// 把物品放到指定插槽中。若插槽内大小不足，那么就只放入一部分的物品。
	// @return 放入物品的数量。
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	virtual int32 AddItemToSlot(UInventoryItemInstance* Item, const FInventoryItemSlotHandle& SlotHandle, int32 Count = 1);
	
	/* 移除插槽中指定数量的物品
	 * @param SlotHandle 要移除哪个插槽的物品
	 * @param Count 指定移除物品的数量
	 * @param OutRemovedItemInfo 已移除的物品信息
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItemFromSlot(const FInventoryItemSlotHandle& SlotHandle, int32 Count, FInventoryRemovedItemInfo& OutRemovedItemInfo);
	
	/** 移除插槽中的所有物品
	 * @param SlotHandle 要移除哪个插槽的物品
	 * @param OutRemovedItemInfo 已移除的物品信息
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveAllItemFromSlot(const FInventoryItemSlotHandle& SlotHandle, FInventoryRemovedItemInfo& OutRemovedItemInfo);
	
	// 判断指定插槽能否放入指定物品
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool IsItemAcceptable(const UInventoryItemInstance* Item, const FInventoryItemSlotHandle& SlotHandle) const;
	
	// 返回所有的物品插槽
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventoryItemSlot> GetAllItemSlots();

	// 返回所有的物品插槽
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventoryItemSlotHandle> GetAllItemSlotHandles();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	FInventoryItemSlotHandle CreateItemSlotHandle(const FInventoryItemSlot& ItemSlot);

protected:
	FInventoryItemSlot* GetItemSlotPtr(const FInventoryItemSlotHandle& Handle);
	const FInventoryItemSlot* GetItemSlotPtr(const FInventoryItemSlotHandle& Handle) const;
	virtual bool IsItemAcceptable(const UInventoryItemInstance* Item, const FInventoryItemSlot& Slot) const;
	
private:
	int32 GenerateNextSlotId() const;
	
public:
	// 新增插槽或移除插槽时会回调
	FOnInventoryUpdatedSignature OnItemSlotAddedOrRemovedDelegate;
	// 插槽内物品变化时会回调
	FOnItemSlotUpdatedSignature OnItemSlotUpdatedDelegate;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<FInventoryItemSlot> Slots;

private:
	mutable TAtomic<int32> SlotIdGenerator = 1;
};
