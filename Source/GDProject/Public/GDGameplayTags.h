// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GDGameplayTags.generated.h"

struct FGameplayTag;

USTRUCT()
struct FGDGameplayTags
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FGameplayTag Attribute_Primary_Strength;
	UPROPERTY()
	FGameplayTag Attribute_Primary_Intelligence;
	UPROPERTY()
	FGameplayTag Attribute_Primary_Resilience;
	UPROPERTY()
	FGameplayTag Attribute_Primary_Vigor;
	
	UPROPERTY()
	FGameplayTag Attribute_Secondary_Armor;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_BlockChange;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_CriticalHitChange;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_MaxHealth;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_MaxMana;
	UPROPERTY()
	FGameplayTag Attribute_Secondary_MaxStamina;

	UPROPERTY()
	FGameplayTag Attribute_Vital_Health;
	UPROPERTY()
	FGameplayTag Attribute_Vital_Mana;
	UPROPERTY()
	FGameplayTag Attribute_Vital_Stamina;


	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	
public:
	static FGDGameplayTags& Get();

	void InitializeGameplayTags();

private:
	// FIXME 不知道为啥，不注释掉这个编译不过
	//FGDGameplayTags();
};
