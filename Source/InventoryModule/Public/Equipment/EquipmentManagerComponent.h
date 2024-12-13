// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/AbilitySet_GameplayAbility.h"
#include "Inventory/InventoryManagerComponent.h"
#include "EquipmentManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FInventoryItemInfoEntry {
	GENERATED_BODY()

	FInventoryItemInfoEntry() {}
	FInventoryItemInfoEntry(const FInventoryItemSlotHandle& InHandle): SlotHandle(InHandle) {}

	UPROPERTY(BlueprintReadOnly)
	FInventoryItemSlotHandle SlotHandle;
	UPROPERTY(BlueprintReadOnly)
	FAbilitySet_GrantHandles AbilitySet_GrantHandles;

	bool operator==(const FInventoryItemInfoEntry& Other) const { return SlotHandle == Other.SlotHandle; }
	bool operator!=(const FInventoryItemInfoEntry& Other) const { return SlotHandle != Other.SlotHandle; }
};

/*
 * 装备栏组件，当物品装备上时，会赋予能力和效果给装备对象
 * 限制一个插槽只能堆叠一次，且只有符合插槽Tag的物品，才能装备进插槽。
 */
UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class INVENTORYMODULE_API UEquipmentManagerComponent : public UInventoryManagerComponent {
	GENERATED_BODY()

public:
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	
	void SetOwnerAbilitySystemComponent(UAbilitySystemComponent* ASC) { OwnerAbilitySystemComponent = ASC; }
	
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() { return OwnerAbilitySystemComponent; }

	virtual FInventoryItemSlotHandle CreateInventorySlot(const FGameplayTagContainer& SlotTags) override;
	virtual FInventoryItemSlotHandle CreateInventorySlotWithMaxStackCount(const FGameplayTagContainer& SlotTags, int32 MaxStackCount) override;
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	FInventoryItemSlotHandle EquipItem(UInventoryItemInstance* Item);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool EquipItemToSlot(UInventoryItemInstance* Item, const FInventoryItemSlotHandle& SlotHandle);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool UnequipItem(FInventoryItemSlotHandle SlotHandle);
	
	virtual int32 AddItem(UInventoryItemInstance* Item, int32 Count) override;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	// 返回满足标签的插槽。相当于 Tag.MatchesAny(SlotTags)
	TArray<FInventoryItemSlot> BP_GetItemSlotsByTag(const FGameplayTag& Tag);
	UFUNCTION(BlueprintCallable, Category="Inventory")
	// 返回满足标签的插槽。相当于为每Tags.HasAny(SlotTags)
	TArray<FInventoryItemSlot> BP_GetItemSlotsByTags(const FGameplayTagContainer& Tags);
private:
	UFUNCTION()
	void OnEquipSlotUpdate(const FItemSlotChangedData& ChangedData);
	
	void Internal_HandleItemEquipped(const FInventoryItemSlotHandle& SlotHandle, UInventoryItemInstance* ItemInstance);
	void Internal_HandleItemUnequipped(const FInventoryItemSlotHandle& SlotHandle);
	
	// 返回满足标签的插槽。相当于 Tag.MatchesAny(SlotTags)
	TArray<FInventoryItemSlot*> GetItemSlotsByTag(const FGameplayTag& Tag);
	// 返回满足标签的插槽。相当于为每Tags.HasAny(SlotTags)
	TArray<FInventoryItemSlot*> GetItemSlotsByTags(const FGameplayTagContainer& Tags);

	// 重写判断条件，增加tags的检查
	virtual bool IsItemAcceptable(const UInventoryItemInstance* Item, const FInventoryItemSlot& Slot) const override;
protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> InitEquipmentSlotTags;
	// 当前的装备
	UPROPERTY()
	TArray<FInventoryItemInfoEntry> EquipmentInfos;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerAbilitySystemComponent;
};
