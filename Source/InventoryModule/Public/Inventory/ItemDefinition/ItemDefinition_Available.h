// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemDefinition.h"
#include "ItemDefinition_Available.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYMODULE_API UItemDefinition_Available : public UInventoryItemDefinition {
	GENERATED_BODY()
public:
	UItemDefinition_Available(const FObjectInitializer& ObjectInitializer);
};
