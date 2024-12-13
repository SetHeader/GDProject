// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/AbilitySet_GameplayAbility.h"
#include "Inventory/InventoryItemFragment.h"
#include "GameplayEffectTypes.h"
#include "InventorySettings.h"
#include "ItemFragment_Equippable.generated.h"

struct FAbilitySet;
struct FAbilitySet_GrantHandles;
class UAbilitySystemComponent;

/**
 * 装备Fragment，如果物品有该Fragment，即表示该物品是个装备
 */
UCLASS()
class INVENTORYMODULE_API UItemFragment_Equippable : public UInventoryItemFragment {
	GENERATED_BODY()
	
public:
	virtual bool GiveToAbilitySystem(UAbilitySystemComponent* ASC, FAbilitySet_GrantHandles* OutHandleStorage, UInventoryItemInstance* SourceObject) const;
	
protected:
	// 装备物品后会给予这些能力
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	FAbilitySet EquipmentAbilitySet;
};
