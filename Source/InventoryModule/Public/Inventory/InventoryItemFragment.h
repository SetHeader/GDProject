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
	/**
	 * 该回调会在InventoryItemInstance初始化的时候会调用。即在UInventoryItemInstance::SetItemDifinition中调用
	 */
	virtual void OnInstancedCreated(UInventoryItemInstance* Instance);

	virtual bool operator==(const UInventoryItemFragment& Other) const;
	virtual bool operator!=(const UInventoryItemFragment& Other) const;
	
protected:
	UPROPERTY()
	TWeakObjectPtr<UInventoryItemInstance> OwnerItemInstance;
};
