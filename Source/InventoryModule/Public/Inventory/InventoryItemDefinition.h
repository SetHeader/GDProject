// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagAssetInterface.h"
#include "InventoryItemDefinition.generated.h"

class UInventoryItemInstance;
class UInventoryItemFragment;

/**
 * 物品的定义
 */
UCLASS(BlueprintType)
class INVENTORYMODULE_API UInventoryItemDefinition : public UPrimaryDataAsset, public IGameplayTagAssetInterface {
	GENERATED_BODY()

public:
	friend class UInventoryItemInstance;
	UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass);

	template<typename FragmentClass>
	requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
	const FragmentClass* FindFragmentByClass();

	TArray<TObjectPtr<UInventoryItemFragment>>& GetFragments() { return ItemFragments; }
	const TArray<TObjectPtr<UInventoryItemFragment>>& GetFragments() const { return ItemFragments; }
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	bool operator==(const UInventoryItemDefinition& Other) const;
	bool operator!=(const UInventoryItemDefinition& Other) const;
protected:
	// 物品名字
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FText ItemName;
	
	// 物品的标签，可以用来指示类型，也可以用来表示交易保护、掉落保护等
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FInheritedTagContainer ItemTags;

	// 该物品的最大堆叠数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxStackCount = 1;
	
	// 该物品拥有的功能
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category="Inventory")
	TArray<TObjectPtr<UInventoryItemFragment>> ItemFragments;
};

template <typename FragmentClass> requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
const FragmentClass* UInventoryItemDefinition::FindFragmentByClass() {
	return Cast<FragmentClass>(FindFragmentByClass(FragmentClass::StaticClass()));
}
