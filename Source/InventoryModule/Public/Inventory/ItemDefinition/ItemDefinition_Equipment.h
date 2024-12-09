// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemDefinition.h"
#include "ItemDefinition_Equipment.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYMODULE_API UItemDefinition_Equipment : public UInventoryItemDefinition {
	GENERATED_BODY()
public:
	UItemDefinition_Equipment(const FObjectInitializer& ObjectInitializer);
};
