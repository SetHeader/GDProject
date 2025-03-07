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
	// 存放所有类型的伤害标签，方便遍历
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	
	/** 各种类型的前缀 */
	FGameplayTag Attribute_Primary;
	FGameplayTag Abilities;
	
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

	FGameplayTag Attribute_Meta_IncomingXP;

	/** 输入相关 */
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;

	/** 伤害类型 */
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	/** 能力相关 */
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Fire_FireBlast;
	FGameplayTag Abilities_Arcane_ArcaneShards;
	
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Lightning_Electrocute;
	FGameplayTag Cooldown_Fire_FireBlast;
	FGameplayTag Cooldown_Arcane_ArcaneShards;

	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	
	// 未解锁
	FGameplayTag Abilities_Status_Locked;
	// 解锁，但不可用
	FGameplayTag Abilities_Status_Eligible;
	// 解锁，且可用
	FGameplayTag Abilities_Status_Unlocked;
	// 解锁，可用，且已装备
	FGameplayTag Abilities_Status_Equipped;
	// 能力类型，主动、被动、未定义
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	// 属性抵抗
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	// Debuff
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;
	
	
	/** 其它 */
	FGameplayTag Effects_HitReact;

	/** Montage */
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	/** 阻塞标签 */
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

	/** 物品主类型 */
	FGameplayTag Inventory_Category_Equipment;	// 装备
	FGameplayTag Inventory_Category_Consumable;	// 消耗品
	FGameplayTag Inventory_Category_Common;		// 普通物品
	/** 物品的次类型 */
	FGameplayTag Inventory_Category_Equipment_Weapon;			// 武器
	FGameplayTag Inventory_Category_Equipment_HeadArmor;		// 头部防具，如帽子、头盔
	FGameplayTag Inventory_Category_Equipment_UpperBodyArmor;	// 上身防具，如布衣、盔甲
	FGameplayTag Inventory_Category_Equipment_LowerBodyArmor;	// 下身防具，如裤子、裙子
	FGameplayTag Inventory_Category_Equipment_UpperLimbArmor;	// 上肢防具，如护腕、手套
	FGameplayTag Inventory_Category_Equipment_LowerLimbArmor;	// 下肢防具，如护膝、鞋子
	FGameplayTag Inventory_Category_Equipment_Accessory;		// 饰品，如项链、戒子


private:
	bool bInitialized = false;

public:
	FOnInitedSignature OnInitedDelegate;
	
	static FGDGameplayTags& Get();

	void InitializeGameplayTags();

	bool IsInitialized() const { return  bInitialized; }

private:
	FORCEINLINE void InitTagVariable(FGameplayTag& CurrentTag, FName TagName, FString Description);
};
