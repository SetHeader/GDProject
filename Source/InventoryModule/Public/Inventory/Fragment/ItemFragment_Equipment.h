// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/AbilitySet_GameplayAbility.h"
#include "Inventory/InventoryItemFragment.h"
#include "GameplayEffectTypes.h"
#include "Inventory/InventorySettings.h"
#include "ItemFragment_Equipment.generated.h"

struct FAbilitySet;
struct FAbilitySet_GrantHandles;
class UAbilitySystemComponent;

// 对某个属性的操作
USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FItemAttributeDefine {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayAttribute Attribute = FGameplayAttribute();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Value = 0;
};

// 属性的容器，包含主属性、次要属性
USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FItemPerkContainer {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FItemAttributeDefine MainAttributeDefine;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FItemAttributeDefine> SecondaryAttributeDefines;
};
// 属性容器的生成器
UCLASS(Abstract, BlueprintType, Blueprintable)
class INVENTORYMODULE_API UItemPerk : public UObject {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FItemPerkContainer GenerateItemPerk();

protected:
	UDataTable* GetItemAttributeDataTable() const;

	FItemAttributeRowDefine* FindItemAttributeRowDefine(FName RowName) const;
};

UCLASS()
class INVENTORYMODULE_API UItemPerk_RandomEquipAttribute : public UItemPerk {
	GENERATED_BODY()

	virtual FItemPerkContainer GenerateItemPerk_Implementation() override;
};

/**
 * 装备Fragment，如果物品有该Fragment，即表示该物品是个装备
 */
UCLASS()
class INVENTORYMODULE_API UItemFragment_Equipment : public UInventoryItemFragment {
	GENERATED_BODY()

public:
	// TODO 逻辑有点怪，让抽象的Instance去依赖具体的Equipment。
	friend class UInventoryItemInstance;
	
	virtual bool GiveToAbilitySystem(UAbilitySystemComponent* ASC, FAbilitySet_GrantHandles* OutHandleStorage, UInventoryItemInstance* SourceObject) const;
	// 根据Perk来动态创建GE
	FGameplayEffectSpecHandle MakeEquipmentPerkEffectSpec(UAbilitySystemComponent* ASC, const FItemPerkContainer& Perk) const;
protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	FAbilitySet EquipmentAbilitySet;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TSoftClassPtr<UItemPerk> EquipmentPerk;
};
