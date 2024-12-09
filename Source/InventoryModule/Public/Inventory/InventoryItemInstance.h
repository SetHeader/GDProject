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
UCLASS()
class INVENTORYMODULE_API UInventoryItemInstance : public UObject, public IGameplayTagAssetInterface {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;

	template<typename FragmentClass>
	requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
	const FragmentClass* FindFragmentByClass() const;
	
	TObjectPtr<UInventoryItemDefinition> GetItemDefinition() { return ItemDefinition; }
	
	void SetItemDifinition(TObjectPtr<UInventoryItemDefinition> InDefinition);

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<UInventoryItemDefinition> ItemDefinition;

	UPROPERTY()
	TArray<TObjectPtr<UInventoryItemFragment>> DynamicFragments;
};

template <typename FragmentClass> requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
const FragmentClass* UInventoryItemInstance::FindFragmentByClass() const {
	return Cast<FragmentClass>(FindFragmentByClass(FragmentClass::StaticClass()));
}
