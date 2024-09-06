// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GDGameplayTags.generated.h"

struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE(FOnInitedSignature);

USTRUCT()
struct FGDGameplayTags
{
	GENERATED_BODY()
public:
	/** 属性相关 */
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;
	
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChange;
	FGameplayTag Attribute_Secondary_CriticalHitChange;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;
	FGameplayTag Attribute_Secondary_MaxStamina;

	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Lighting;
	FGameplayTag Attribute_Resistance_Arcane;
	FGameplayTag Attribute_Resistance_Physical;

	FGameplayTag Attribute_Vital_Health;
	FGameplayTag Attribute_Vital_Mana;
	FGameplayTag Attribute_Vital_Stamina;

	/** 输入相关 */
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	/** 伤害类型 */
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lighting;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	// 存放所有类型的伤害标签，方便遍历
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;
	
	/** 其它 */
	FGameplayTag Effects_HitReact;

private:
	bool bInitialized = false;

public:
	FOnInitedSignature OnInitedDelegate;
	
	static FGDGameplayTags& Get();

	void InitializeGameplayTags();

	bool IsInitialized() const { return  bInitialized; }
};
