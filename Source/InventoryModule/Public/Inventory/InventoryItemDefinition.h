// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagAssetInterface.h"
#include "InventoryItemDefinition.generated.h"

class UInventoryItemFragment;

/**
 * 物品的定义
 */
UCLASS(BlueprintType)
class INVENTORYMODULE_API UInventoryItemDefinition : public UPrimaryDataAsset, public IGameplayTagAssetInterface {
	GENERATED_BODY()

public:
	UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass);

	template<typename FragmentClass>
	requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
	const FragmentClass* FindFragmentByClass();

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FInheritedTagContainer ItemTags;

	// TODO 既然是定义，为什么不用 TSubclassOf 而是用 TObjectPtr
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category="Inventory")
	TArray<TObjectPtr<UInventoryItemFragment>> Fragments;

	
};

template <typename FragmentClass> requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
const FragmentClass* UInventoryItemDefinition::FindFragmentByClass() {
	return Cast<FragmentClass>(FindFragmentByClass(FragmentClass::StaticClass()));
}
