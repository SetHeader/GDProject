// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/AbilitySet_GameplayAbility.h"
#include "Inventory/InventoryItemFragment.h"
#include "ItemFragment_Available.generated.h"

class UInventoryManagerComponent;

USTRUCT(BlueprintType)
struct FUseItemParams {
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	AActor* TargetActor = nullptr;
	UPROPERTY(BlueprintReadWrite)
	AActor* SourceActor = nullptr;
	UPROPERTY(BlueprintReadWrite)
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY(BlueprintReadWrite)
	UAbilitySystemComponent* SourceASC = nullptr;
	
};

/**
 * 可以使用的功能，如药水、技能书等
 */
UCLASS()
class INVENTORYMODULE_API UItemFragment_Available : public UInventoryItemFragment {
	GENERATED_BODY()
	
public:
	virtual bool UseItem(const FUseItemParams& Params);
	
private:
	virtual bool GiveToAbilitySystem(UAbilitySystemComponent* ASC, FAbilitySet_GrantHandles* OutHandleStorage) const;
	
protected:
	// 装备物品后会给予这些能力
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	FAbilitySet EquipmentAbilitySet;
	
	// 使用后从背包中移除
	bool bOnlyUseOnce = true;
};
