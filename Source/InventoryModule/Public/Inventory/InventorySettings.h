// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayEffectTypes.h"
#include "ScalableFloat.h"
#include "InventorySettings.generated.h"

USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FItemAttributeRowDefine : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGameplayAttribute Attribute;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FScalableFloat Magnitude;
};

/**
 * 物品系统的配置项
 */
UCLASS(Config = Game, DefaultConfig)
class INVENTORYMODULE_API UInventorySettings : public UDeveloperSettings {
	GENERATED_BODY()
public:
	UInventorySettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", Config)
	TSoftObjectPtr<UDataTable> EquipmentAttributeDefineTable;
};
