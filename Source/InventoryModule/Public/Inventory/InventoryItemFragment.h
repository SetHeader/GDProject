// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventoryItemFragment.generated.h"

class UInventoryItemInstance;
/**
 * 物品属性
 */
UCLASS()
class INVENTORYMODULE_API UInventoryItemFragment : public UObject {
	GENERATED_BODY()

public:
	virtual void OnInstancedCreated(UInventoryItemInstance* Instance) const {}
};
