// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet_GameplayAbility.h"
#include "InventoryComponent.h"
#include "InventoryComponent_Equipment.generated.h"

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
 */
UCLASS(ClassGroup=(Inventory), meta=(BlueprintSpawnableComponent))
class INVENTORYMODULE_API UInventoryComponent_Equipment : public UInventoryComponent {
	GENERATED_BODY()

public:
	UInventoryComponent_Equipment(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;

	void SetOwnerAbilitySystemComponent(UAbilitySystemComponent* ASC) { OwnerAbilitySystemComponent = ASC; }
	
	UAbilitySystemComponent* GetOwnerAbilitySystemComponent() { return OwnerAbilitySystemComponent; }

protected:
	UFUNCTION()
	virtual void OnEquipSlotUpdate(UInventoryComponent* InventoryComponent, const FInventoryItemSlotHandle& SlotHandle,
			UInventoryItemInstance* ItemInstance, UInventoryItemInstance* PreviousItemInstance);

	virtual bool MakeItemEquipped_Internal(const FInventoryItemSlotHandle& SlotHandle);
	virtual bool MakeItemEquipped_Internal(const FInventoryItemSlotHandle& SlotHandle, UInventoryItemInstance* ItemInstance);
	virtual bool MakeItemUnequipped_Internal(const FInventoryItemSlotHandle& SlotHandle);
	virtual bool MakeItemUnequipped_Internal(const FInventoryItemSlotHandle& SlotHandle, UInventoryItemInstance* ItemInstance);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> InitEquipmentSlotTags;
	// 当前的装备
	UPROPERTY()
	TArray<FInventoryItemInfoEntry> EquipmentInfos;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> OwnerAbilitySystemComponent;
};
