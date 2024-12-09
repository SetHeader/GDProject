// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemFragment_Equipment.h"
#include "ItemFragment_EquipmentInstance.generated.h"

// TODO 有点怪，为什么还要一个具体的Fragment实例类？
/**
 * 装备Fragment的实例
 */
UCLASS()
class INVENTORYMODULE_API UItemFragment_EquipmentInstance : public UItemFragment_Equipment {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FItemPerkContainer PerkContainer;
};
