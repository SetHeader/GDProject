// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment_Equippable.h"
#include "ItemFragment_Equippable_WithPerk.generated.h"

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
 * 可装备的物品片段。在原有功能的基础上提供了随机属性的功能。
 */
UCLASS()
class INVENTORYMODULE_API UItemFragment_Equippable_WithPerk : public UItemFragment_Equippable {
	GENERATED_BODY()

public:
	/**
	 * 把当前定义的能力给予到ASC中，并把能力句柄存放到OutHandleStorage中
	 * @return 能力是否给予成功
	 */
	virtual bool GiveToAbilitySystem(UAbilitySystemComponent* ASC, FAbilitySet_GrantHandles* OutHandleStorage, UInventoryItemInstance* SourceObject) const override;

	virtual void OnInstancedCreated(UInventoryItemInstance* Instance) override;
	
	// 根据Perk来动态创建GE，并把生成的随机属性存放到OutPerkContainer中
	FGameplayEffectSpecHandle MakeEquipmentPerkEffectSpec(UAbilitySystemComponent* ASC, const FItemPerkContainer& OutPerkContainer) const;
	
private:
	// 用于生成随机属性的工具类
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSoftClassPtr<UItemPerk> EquipmentPerk;
	
	// 随机属性生成后的存放容器
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	FItemPerkContainer PerkContainer;
};
