// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/ItemDefinition/ItemDefinition_Equipment.h"

#include "Inventory/Fragment/ItemFragment_Equipment.h"
#include "Inventory/Fragment/ItemFragment_Icon.h"

UItemDefinition_Equipment::UItemDefinition_Equipment(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
	Fragments.Add(CreateDefaultSubobject<UItemFragment_Icon>("ItemFragment_Icon"));
	Fragments.Add(CreateDefaultSubobject<UItemFragment_Equipment>("ItemFragment_Equipment"));
}
