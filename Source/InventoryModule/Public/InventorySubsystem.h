// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventorySubsystem.generated.h"

class UInventoryItemDefinition;
class UInventoryItemInstance;
/**
 * 物品管理子系统，提供了便利的方法来操作物品
 */
UCLASS()
class INVENTORYMODULE_API UInventorySubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()
public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem
};
