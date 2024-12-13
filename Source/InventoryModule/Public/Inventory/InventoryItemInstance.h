// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "UObject/Object.h"
#include "InventoryItemInstance.generated.h"

class UInventoryItemFragment;
class UInventoryItemDefinition;
/**
 * 物品实例
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYMODULE_API UInventoryItemInstance : public UObject, public IGameplayTagAssetInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass);

	template<typename FragmentClass>
	requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
	FragmentClass* FindFragmentByClass();
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UInventoryItemFragment*> GetFragments();
	
	TObjectPtr<UInventoryItemDefinition> GetItemDefinition() { return ItemDefinition; }
	
	UFUNCTION(BlueprintGetter, Category = "Inventory")
	int32 GetMaxStackCount() const;
	
	UFUNCTION(BlueprintGetter, Category = "Inventory")
	FText GetItemName() const;

	UFUNCTION(BlueprintGetter, Category = "Inventory")
	FGameplayTagContainer GetItemTags() const;
	
	// 设置物品定义，内部会创建定义的实例
	void SetItemDifinition(TSubclassOf<UInventoryItemDefinition> InDefinition);
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
	virtual FString ToString() const;
	
	bool operator==(const UInventoryItemInstance& Other) const;
	
protected:
	// 该实例不是CDO对象，是NewObject出来对象
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryItemDefinition> ItemDefinition;
};

template <typename FragmentClass> requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
FragmentClass* UInventoryItemInstance::FindFragmentByClass() {
	return Cast<FragmentClass>(FindFragmentByClass(FragmentClass::StaticClass()));
}
