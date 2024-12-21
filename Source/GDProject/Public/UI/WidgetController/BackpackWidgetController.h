// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryStatics.h"
#include "UI/Base/WidgetController.h"
#include "BackpackWidgetController.generated.h"

struct FInventoryItemSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRightClickedSignature, UUserWidget*, TargetWidget);
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
	void OnRightClicked(UUserWidget* TargetWidget);
	
protected:
	UPROPERTY(BlueprintAssignable, Category = "BackpackWidgetController")
	FOnRightClickedSignature OnRightClickedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "BackpackWidgetController")
	FOnSlotInfoChangedSignature OnSlotInfoChangedDelegate;

	UPROPERTY(BlueprintReadOnly, Category = "BackpackWidgetController")
	UUserWidget* PreviousRightClickedWidget;
	UPROPERTY(BlueprintReadOnly, Category = "BackpackWidgetController")
	UUserWidget* CurrentRightClickedWidget;

private:
	FDelegateHandle OnItemSlotChangedHandle;
};
