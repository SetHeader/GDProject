// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryItemFragment.h"
#include "ItemFragment_Pickupable.generated.h"

/**
 * 可拾取的物品
 */
UCLASS(BlueprintType)
class INVENTORYMODULE_API UItemFragment_Pickupable : public UInventoryItemFragment {
	GENERATED_BODY()

public:
	UItemFragment_Pickupable();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Inventory)
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Inventory)
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Inventory)
	FLinearColor PadColor;
};
