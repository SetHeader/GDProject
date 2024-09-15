// Fill out your copyright notice in the Description page of Project Settings.


#include "GDGameplayTags.h"
#include <GameplayTagsManager.h>

FGDGameplayTags& FGDGameplayTags::Get()
{
	static FGDGameplayTags Instance;
	return Instance;
}

void FGDGameplayTags::InitializeGameplayTags()
{
	/*
	 * Primary Attribute Tag
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Strength"), FString("This is a Strength"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Intelligence"), FString("This is a Intelligence"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Resilience"), FString("This is a Resilience"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Primary.Vigor"), FString("This is a Vigor"));
	Attribute_Primary_Strength = FGameplayTag::RequestGameplayTag(FName("Attribute.Primary.Strength"));
	Attribute_Primary_Intelligence = FGameplayTag::RequestGameplayTag(FName("Attribute.Primary.Intelligence"));
	Attribute_Primary_Resilience = FGameplayTag::RequestGameplayTag(FName("Attribute.Primary.Resilience"));
	Attribute_Primary_Vigor = FGameplayTag::RequestGameplayTag(FName("Attribute.Primary.Vigor"));
	
	/*
	 * Secondary Attribute Tag
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.Armor"), FString("This is a Armor"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ArmorPenetration"), FString("This is a ArmorPenetration"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.BlockChange"), FString("This is a BlockChange"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitChange"), FString("This is a CriticalHitChange"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitDamage"), FString("This is a CriticalHitDamage"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.CriticalHitResistance"), FString("This is a CriticalHitResistance"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.HealthRegeneration"), FString("This is a HealthRegeneration"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.ManaRegeneration"), FString("This is a ManaRegeneration"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxHealth"), FString("This is a MaxHealth"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxMana"), FString("This is a MaxMana"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Secondary.MaxStamina"), FString("This is a MaxStamina"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Health"), FString("This is a Health"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Mana"), FString("This is a Mana"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Vital.Stamina"), FString("This is a Stamina"));
	Attribute_Secondary_Armor = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.Armor"));
	Attribute_Secondary_ArmorPenetration = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.ArmorPenetration"));
	Attribute_Secondary_BlockChange = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.BlockChange"));
	Attribute_Secondary_CriticalHitChange = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.CriticalHitChange"));
	Attribute_Secondary_CriticalHitDamage = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.CriticalHitDamage"));
	Attribute_Secondary_CriticalHitResistance = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.CriticalHitResistance"));
	Attribute_Secondary_HealthRegeneration = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.HealthRegeneration"));
	Attribute_Secondary_ManaRegeneration = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.ManaRegeneration"));
	Attribute_Secondary_MaxHealth = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.MaxHealth"));
	Attribute_Secondary_MaxMana = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.MaxMana"));
	Attribute_Secondary_MaxStamina = FGameplayTag::RequestGameplayTag(FName("Attribute.Secondary.MaxStamina"));
	Attribute_Vital_Health = FGameplayTag::RequestGameplayTag(FName("Attribute.Vital.Health"));
	Attribute_Vital_Mana = FGameplayTag::RequestGameplayTag(FName("Attribute.Vital.Mana"));
	Attribute_Vital_Stamina = FGameplayTag::RequestGameplayTag(FName("Attribute.Vital.Stamina"));
	
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Fire"), FString("This is a Attribute_Resistance_Fire"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Lighting"), FString("This is a Attribute_Resistance_Lighting"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Arcane"), FString("This is a Attribute_Resistance_Arcane"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Resistance.Physical"), FString("This is a Attribute_Resistance_Physical"));
	Attribute_Resistance_Fire = FGameplayTag::RequestGameplayTag(FName("Attribute.Resistance.Fire"));
	Attribute_Resistance_Lighting = FGameplayTag::RequestGameplayTag(FName("Attribute.Resistance.Lighting"));
	Attribute_Resistance_Arcane = FGameplayTag::RequestGameplayTag(FName("Attribute.Resistance.Arcane"));
	Attribute_Resistance_Physical = FGameplayTag::RequestGameplayTag(FName("Attribute.Resistance.Physical"));
	
	/*
	 * Input Tag
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag For Left Mouse Button"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag For Right Mouse Button"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Input Tag For 1 Key"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Input Tag For 2 Key"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("Input Tag For 3 Key"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("Input Tag For 4 Key"));
	InputTag_LMB = FGameplayTag::RequestGameplayTag(FName("InputTag.LMB"));
	InputTag_RMB = FGameplayTag::RequestGameplayTag(FName("InputTag.RMB"));
	InputTag_1 = FGameplayTag::RequestGameplayTag(FName("InputTag.1"));
	InputTag_2 = FGameplayTag::RequestGameplayTag(FName("InputTag.2"));
	InputTag_3 = FGameplayTag::RequestGameplayTag(FName("InputTag.3"));
	InputTag_4 = FGameplayTag::RequestGameplayTag(FName("InputTag.4"));

	/*
	 * 伤害
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Damage Fire"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lighting"), FString("Damage Lighting"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString("Damage Arcane"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Damage Physical"));
	Damage_Fire = FGameplayTag::RequestGameplayTag(FName("Damage.Fire"));
	Damage_Lighting = FGameplayTag::RequestGameplayTag(FName("Damage.Lighting"));
	Damage_Arcane = FGameplayTag::RequestGameplayTag(FName("Damage.Arcane"));
	Damage_Physical = FGameplayTag::RequestGameplayTag(FName("Damage.Physical"));

	DamageTypesToResistance.Add(Damage_Fire, Attribute_Resistance_Fire);
	DamageTypesToResistance.Add(Damage_Lighting, Attribute_Resistance_Lighting);
	DamageTypesToResistance.Add(Damage_Arcane, Attribute_Resistance_Arcane);
	DamageTypesToResistance.Add(Damage_Physical, Attribute_Resistance_Physical);
	
	/*
	 * 击中反应
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("HitReact"));
	Effects_HitReact = FGameplayTag::RequestGameplayTag(FName("Effects.HitReact"));
	
	/*
	 * 能力相关
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Attack Ability Tag"));
	Abilities_Attack = FGameplayTag::RequestGameplayTag(FName("Abilities.Attack"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Summon Ability Tag"));
	Abilities_Summon = FGameplayTag::RequestGameplayTag(FName("Abilities.Summon"));
	/*
	 * Montage
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("Combat Socket Weapon"));
	CombatSocket_Weapon = FGameplayTag::RequestGameplayTag(FName("CombatSocket.Weapon"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString("Combat Socket RightHand"));
	CombatSocket_RightHand = FGameplayTag::RequestGameplayTag(FName("CombatSocket.RightHand"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString("Combat Socket LeftHand"));
	CombatSocket_LeftHand = FGameplayTag::RequestGameplayTag(FName("CombatSocket.LeftHand"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), FString("Combat Socket Tail"));
	CombatSocket_Tail = FGameplayTag::RequestGameplayTag(FName("CombatSocket.Tail"));


	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Montage Attack 1"));
	Montage_Attack_1 = FGameplayTag::RequestGameplayTag(FName("Montage.Attack.1"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Montage Attack 2"));
	Montage_Attack_2 = FGameplayTag::RequestGameplayTag(FName("Montage.Attack.2"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Montage Attack 3"));
	Montage_Attack_3 = FGameplayTag::RequestGameplayTag(FName("Montage.Attack.3"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Montage Attack 4"));
	Montage_Attack_4 = FGameplayTag::RequestGameplayTag(FName("Montage.Attack.4"));
	
	/*
	 * 广播初始化完毕
	 */
	OnInitedDelegate.Broadcast();
}
