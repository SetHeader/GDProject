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
	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;
	
	template<typename FragmentClass>
	requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
	const FragmentClass* FindFragmentByClass() const;
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UInventoryItemFragment*> GetFragments() const;
	
	const UInventoryItemDefinition* GetItemDefinition() const;
	
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
	
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// 该实例就是CDO
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TSubclassOf<UInventoryItemDefinition> ItemDefinition;
};

template <typename FragmentClass> requires std::is_base_of_v<UInventoryItemFragment, FragmentClass>
const FragmentClass* UInventoryItemInstance::FindFragmentByClass() const {
	return Cast<FragmentClass>(FindFragmentByClass(FragmentClass::StaticClass()));
}
