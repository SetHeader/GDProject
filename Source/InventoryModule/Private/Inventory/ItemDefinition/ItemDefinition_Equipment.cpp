﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemDefinition/ItemDefinition_Equipment.h"

#include "Inventory/Fragment/ItemFragment_BaseInfo.h"
#include "Inventory/Fragment/ItemFragment_Equippable.h"
#include "Inventory/Fragment/ItemFragment_Pickupable.h"

UItemDefinition_Equipment::UItemDefinition_Equipment(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	ItemFragments.Add(CreateDefaultSubobject<UItemFragment_BaseInfo>("ItemFragment_BaseInfo"));
	ItemFragments.Add(CreateDefaultSubobject<UItemFragment_Equippable>("ItemFragment_Equippable"));
	ItemFragments.Add(CreateDefaultSubobject<UItemFragment_Pickupable>("ItemFragment_Pickupable"));
}
