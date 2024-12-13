// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemDefinition/ItemDefinition_Available.h"

#include "Inventory/Fragment/ItemFragment_Available.h"
#include "Inventory/Fragment/ItemFragment_BaseInfo.h"
#include "Inventory/Fragment/ItemFragment_Equippable.h"
#include "Inventory/Fragment/ItemFragment_Pickupable.h"

UItemDefinition_Available::UItemDefinition_Available(const FObjectInitializer& ObjectInitializer) {
	ItemFragments.Add(CreateDefaultSubobject<UItemFragment_BaseInfo>("ItemFragment_BaseInfo"));
	ItemFragments.Add(CreateDefaultSubobject<UItemFragment_Available>("ItemFragment_Available"));
	ItemFragments.Add(CreateDefaultSubobject<UItemFragment_Pickupable>("ItemFragment_Pickupable"));
}
