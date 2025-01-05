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
	
	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;
	UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass);
	
	template<typename FragmentClass>
	requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
	const FragmentClass* FindFragmentByClass() const;
	
	template<typename FragmentClass>
	requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
	FragmentClass* FindFragmentByClass();

	TArray<TObjectPtr<UInventoryItemFragment>>& GetFragments() { return ItemFragments; }
	const TArray<TObjectPtr<UInventoryItemFragment>>& GetFragments() const { return ItemFragments; }
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	bool operator==(const UInventoryItemDefinition& Other) const;
	bool operator!=(const UInventoryItemDefinition& Other) const;

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// 物品名字
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FText ItemName;
	
	// 物品的标签，可以用来指示类型，也可以用来表示交易保护、掉落保护等
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	FInheritedTagContainer ItemTags;
	
	// 该物品的最大堆叠数量
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxStackCount = 1;
	
	// 该物品拥有的功能
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Instanced, Category="Inventory")
	TArray<TObjectPtr<UInventoryItemFragment>> ItemFragments;
};

template <typename FragmentClass> requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
const FragmentClass* UInventoryItemDefinition::FindFragmentByClass() const {
	return Cast<FragmentClass>(FindFragmentByClass(FragmentClass::StaticClass()));
}

template <typename FragmentClass> requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
FragmentClass* UInventoryItemDefinition::FindFragmentByClass() {
	return Cast<FragmentClass>(FindFragmentByClass(FragmentClass::StaticClass()));
}
