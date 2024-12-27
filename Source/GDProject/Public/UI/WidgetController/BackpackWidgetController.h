// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryStatics.h"
#include "Inventory/InventoryItemSlot.h"
#include "UI/Base/WidgetController.h"
#include "BackpackWidgetController.generated.h"

struct FInventoryItemSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRightClickedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotInfoChangedSignature, const FItemSlotInfo&, ChangedItemSlotInfo);

UCLASS()
class GDPROJECT_API UBackpackWidgetController : public UWidgetController {
	GENERATED_BODY()

public:
	virtual void OnInitialized() override;
	virtual void OnDeinitialized() override;
	
	// 返回所有的物品插槽
	UFUNCTION(BlueprintCallable, Category = "BackpackWidgetController")
	TArray<FInventoryItemSlot> GetAllItemSlots();
	
	// 返回所有的物品插槽
	UFUNCTION(BlueprintCallable, Category = "BackpackWidgetController")
	TArray<FInventoryItemSlotHandle> GetAllItemSlotHandles();
	
	UFUNCTION(BlueprintCallable, Category = "BackpackWidgetController")
	void UseItem(UAbilitySystemComponent* TargetASC, FInventoryItemSlotHandle SlotHandle, int32 AmountToUse = 1);
	
	UFUNCTION(BlueprintCallable, Category = "BackpackWidgetController")
	void EquipItem(FInventoryItemSlotHandle SlotHandle);

	// 移动插槽内的物品到指定插槽
	UFUNCTION(BlueprintCallable, Category = "BackpackWidgetController")
	void MoveItem(FInventoryItemSlotHandle FromSlotHandle, FInventoryItemSlotHandle ToSlotHandle, int32 AmountToMove = 1);
	
	UFUNCTION(BlueprintCallable, Category = "BackpackWidgetController")
	void OnRightClicked(UUserWidget* UserWidget, FInventoryItemSlotHandle SlotHandle);
	
protected:
	UPROPERTY(BlueprintAssignable, Category = "BackpackWidgetController")
	FOnRightClickedSignature OnRightClickedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "BackpackWidgetController")
	FOnSlotInfoChangedSignature OnSlotInfoChangedDelegate;
	// 右键菜单弹出时，前一个控件
	UPROPERTY(BlueprintReadOnly, Category = "BackpackWidgetController")
	UUserWidget* PreviousUserWidget;
	// 右键菜单弹出时，当前控件
	UPROPERTY(BlueprintReadOnly, Category = "BackpackWidgetController")
	UUserWidget* CurrentUserWidget;
	// 右键菜单弹出时，当前选择的插槽的句柄
	UPROPERTY(BlueprintReadOnly, Category = "BackpackWidgetController")
	FInventoryItemSlotHandle CurrentItemSlotHandle;
	// 右键菜单弹出时，当前选择的插槽的信息
	UPROPERTY(BlueprintReadOnly, Category = "BackpackWidgetController")
	FItemSlotInfo CurrentItemSlotInfo;

	// 当前悬浮的物品信息
	UPROPERTY(BlueprintReadWrite, Category = "BackpackWidgetController")
	FItemSlotInfo CurrentHoverSlotInfo;
private:
	FDelegateHandle OnItemSlotChangedHandle;
};
