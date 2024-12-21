// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryStatics.generated.h"

struct FInventoryItemSlot;
struct FUseItemParams;
class UAbilitySystemComponent;
struct FInventoryItemSlotHandle;
class UInventoryItemDefinition;
class UInventoryManagerComponent;

USTRUCT(BlueprintType)
struct FItemSlotInfo {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "ItemSlotInfo")
	int32 SlotId = 0;
	
	// 插槽tag，可以指定插槽类型，如武器、防具等
	UPROPERTY(BlueprintReadOnly, Category = "ItemSlotInfo")
	FGameplayTagContainer SlotTags = FGameplayTagContainer::EmptyContainer;
	
	UPROPERTY(BlueprintReadOnly, Category = "ItemSlotInfo")
	bool ContainItem = false;
	
	// 物品名字
	UPROPERTY(BlueprintReadOnly, Category = "ItemSlotInfo")
	FText ItemName;
	
	// 物品的标签，可以用来指示类型，也可以用来表示交易保护、掉落保护等
	UPROPERTY(BlueprintReadOnly, Category = "ItemSlotInfo")
	FGameplayTagContainer ItemTags;

	// 物品堆叠数量
	UPROPERTY(BlueprintReadOnly, Category = "ItemSlotInfo")
	int32 StackCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "ItemSlotInfo")
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditDefaultsOnly, Category = "ItemSlotInfo")
	FText Description;
};

/**
 * 提供物品系统的便捷方法
 */
UCLASS()
class INVENTORYMODULE_API UInventoryStatics : public UBlueprintFunctionLibrary {
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventorySystem")
	static UInventoryManagerComponent* GetInventoryManagerComponent(AActor* Actor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventorySystem")
	static UEquipmentManagerComponent* GetEquipmentManagerComponent(AActor* Actor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="InventorySystem")
	static UInventoryItemInstance* GenerateItemInstance(TSubclassOf<UInventoryItemDefinition> ItemDefinition);

	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	static FItemSlotInfo GetItemSlotInfo(FInventoryItemSlotHandle SlotHandle);
	// 使用插槽内的物品
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	static void UseItem(UAbilitySystemComponent* TargetASC, FInventoryItemSlotHandle SlotHandle, int32 AmountToUse = 1);
	// 移动插槽内的物品到指定插槽
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	static void MoveItem(FInventoryItemSlotHandle FromSlotHandle, FInventoryItemSlotHandle ToSlotHandle, int32 AmountToMove = 1);
	// 交换两个插槽的物品
	UFUNCTION(BlueprintCallable, Category = "InventorySystem")
	static void SwapItem(FInventoryItemSlotHandle FromSlotHandle, FInventoryItemSlotHandle ToSlotHandle);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventorySystem")
	static FUseItemParams MakeUseItemParams(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventorySystem")
	static FUseItemParams MakeUseItemParamsWithSourceActor(AActor* SourceActor, AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "InventorySystem")
	static FString StringifyItemSlot(const FInventoryItemSlot& ItemSlot);
};
