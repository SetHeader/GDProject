// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemFragment.h"
#include "ItemFragment_BaseInfo.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class INVENTORYMODULE_API UItemFragment_BaseInfo : public UInventoryItemFragment {
	GENERATED_BODY()

	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FText Description;
};
