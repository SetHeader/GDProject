// Fill out your copyright notice in the Description page of Project Settings.


#include "GDGameplayTags.h"
#include <GameplayTagsManager.h>

FGDGameplayTags& FGDGameplayTags::Get()
{
	static FGDGameplayTags Instance;
	return Instance;
}

void FGDGameplayTags::InitTagVariable(FGameplayTag& CurrentTag, FName TagName, FString Description)
{
	UGameplayTagsManager::Get().AddNativeGameplayTag(TagName, Description);
	CurrentTag = FGameplayTag::RequestGameplayTag(TagName);
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
	
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attribute.Meta.IncomingXP"), FString("This is a Attribute_Meta_IncomingXP"));
	Attribute_Meta_IncomingXP = FGameplayTag::RequestGameplayTag(FName("Attribute.Meta.IncomingXP"));
	
	/*
	 * Input Tag
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag For Left Mouse Button"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag For Right Mouse Button"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Input Tag For 1 Key"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Input Tag For 2 Key"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("Input Tag For 3 Key"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("Input Tag For 4 Key"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"), FString("Input Tag For Passive 1"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"), FString("Input Tag For Passive 2"));
	InputTag_LMB = FGameplayTag::RequestGameplayTag(FName("InputTag.LMB"));
	InputTag_RMB = FGameplayTag::RequestGameplayTag(FName("InputTag.RMB"));
	InputTag_1 = FGameplayTag::RequestGameplayTag(FName("InputTag.1"));
	InputTag_2 = FGameplayTag::RequestGameplayTag(FName("InputTag.2"));
	InputTag_3 = FGameplayTag::RequestGameplayTag(FName("InputTag.3"));
	InputTag_4 = FGameplayTag::RequestGameplayTag(FName("InputTag.4"));
	InputTag_Passive_1 = FGameplayTag::RequestGameplayTag(FName("InputTag.Passive.1"));
	InputTag_Passive_2 = FGameplayTag::RequestGameplayTag(FName("InputTag.Passive.2"));
	/*
	 * 伤害
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Damage Fire"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Damage Lightning"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString("Damage Arcane"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Damage Physical"));
	Damage_Fire = FGameplayTag::RequestGameplayTag(FName("Damage.Fire"));
	Damage_Lightning = FGameplayTag::RequestGameplayTag(FName("Damage.Lightning"));
	Damage_Arcane = FGameplayTag::RequestGameplayTag(FName("Damage.Arcane"));
	Damage_Physical = FGameplayTag::RequestGameplayTag(FName("Damage.Physical"));

	/*
	 * 能力类型
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"), FString("Abilities Type Offensive"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"), FString("Abilities Type Passive"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"), FString("Abilities Type None"));
	Abilities_Type_Offensive = FGameplayTag::RequestGameplayTag(FName("Abilities.Type.Offensive"));
	Abilities_Type_Passive = FGameplayTag::RequestGameplayTag(FName("Abilities.Type.Passive"));
	Abilities_Type_None = FGameplayTag::RequestGameplayTag(FName("Abilities.Type.None"));

	/*
	 * 能力
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("Abilities_Fire_FireBolt"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"), FString("Abilities_Lightning_Electrocute"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBlast"), FString("Abilities_Fire_FireBlast"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneShards"), FString("Abilities_Arcane_ArcaneShards"));
	Abilities_Fire_FireBolt = FGameplayTag::RequestGameplayTag(FName("Abilities.Fire.FireBolt"));
	Abilities_Lightning_Electrocute = FGameplayTag::RequestGameplayTag(FName("Abilities.Lightning.Electrocute"));
	Abilities_Fire_FireBlast = FGameplayTag::RequestGameplayTag(FName("Abilities.Fire.FireBlast"));
	Abilities_Arcane_ArcaneShards = FGameplayTag::RequestGameplayTag(FName("Abilities.Arcane.ArcaneShards"));

	/*
	 * 能力冷却
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("Cooldown_Fire_FireBolt"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Lightning.Electrocute"), FString("Cooldown_Lightning_Electrocute"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBlast"), FString("Cooldown_Fire_FireBlast"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Arcane.ArcaneShards"), FString("Cooldown_Arcane_ArcaneShards"));
	Cooldown_Fire_FireBolt = FGameplayTag::RequestGameplayTag(FName("Cooldown.Fire.FireBolt"));
	Cooldown_Lightning_Electrocute = FGameplayTag::RequestGameplayTag(FName("Cooldown.Lightning.Electrocute"));
	Cooldown_Fire_FireBlast = FGameplayTag::RequestGameplayTag(FName("Cooldown.Fire.FireBlast"));
	Cooldown_Arcane_ArcaneShards = FGameplayTag::RequestGameplayTag(FName("Cooldown.Arcane.ArcaneShards"));
	
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

	/**
	 *	能力的状态
	 */
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString("Abilities Status Locked"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString("Abilities Status Eligible"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString("Abilities Status Unlocked"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString("Abilities Status Equipped"));
	Abilities_Status_Locked = FGameplayTag::RequestGameplayTag(FName("Abilities.Status.Locked"));
	Abilities_Status_Eligible = FGameplayTag::RequestGameplayTag(FName("Abilities.Status.Eligible"));
	Abilities_Status_Unlocked = FGameplayTag::RequestGameplayTag(FName("Abilities.Status.Unlocked"));
	Abilities_Status_Equipped = FGameplayTag::RequestGameplayTag(FName("Abilities.Status.Equipped"));

	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Attributes_Resistance_Fire"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Attributes_Resistance_Lightning"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Attributes_Resistance_Arcane"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Attributes_Resistance_Physical"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"), FString("Debuff_Burn"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"), FString("Debuff_Stun"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Arcane"), FString("Debuff_Arcane"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Physical"), FString("Debuff_Physical"));
	Attributes_Resistance_Fire = FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Fire"));
	Attributes_Resistance_Lightning = FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Lightning"));
	Attributes_Resistance_Arcane = FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Arcane"));
	Attributes_Resistance_Physical = FGameplayTag::RequestGameplayTag(FName("Attributes.Resistance.Physical"));
	Debuff_Burn = FGameplayTag::RequestGameplayTag(FName("Debuff.Burn"));
	Debuff_Stun = FGameplayTag::RequestGameplayTag(FName("Debuff.Stun"));
	Debuff_Arcane = FGameplayTag::RequestGameplayTag(FName("Debuff.Arcane"));
	Debuff_Physical = FGameplayTag::RequestGameplayTag(FName("Debuff.Physical"));

	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"), FString("Debuff_Chance"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"), FString("Debuff_Damage"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"), FString("Debuff_Duration"));
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Frequency"), FString("Debuff_Frequency"));
	Debuff_Chance = FGameplayTag::RequestGameplayTag(FName("Debuff.Chance"));
	Debuff_Damage = FGameplayTag::RequestGameplayTag(FName("Debuff.Damage"));
	Debuff_Duration = FGameplayTag::RequestGameplayTag(FName("Debuff.Duration"));
	Debuff_Frequency = FGameplayTag::RequestGameplayTag(FName("Debuff.Frequency"));

	InitTagVariable(Player_Block_InputPressed, "Player.Block.InputPressed", "Player_Block_InputPressed");
	InitTagVariable(Player_Block_InputHeld, "Player.Block.InputHeld", "Player_Block_InputHeld");
	InitTagVariable(Player_Block_InputReleased, "Player.Block.InputReleased", "Player_Block_InputReleased");
	InitTagVariable(Player_Block_CursorTrace, "Player.Block.CursorTrace", "Player_Block_CursorTrace");

	InitTagVariable(Abilities_Passive_HaloOfProtection, "Abilities.Passive.HaloOfProtection", "Abilities_Passive_HaloOfProtection");
	InitTagVariable(Abilities_Passive_LifeSiphon, "Abilities.Passive.LifeSiphon", "Abilities_Passive_LifeSiphon");
	InitTagVariable(Abilities_Passive_ManaSiphon, "Abilities.Passive.ManaSiphon", "Abilities_Passive_ManaSiphon");
	/*
	 * Map of Damage Types to Resistances
	 */
	DamageTypesToResistances.Add(Damage_Arcane, Attributes_Resistance_Arcane);
	DamageTypesToResistances.Add(Damage_Lightning, Attributes_Resistance_Lightning);
	DamageTypesToResistances.Add(Damage_Physical, Attributes_Resistance_Physical);
	DamageTypesToResistances.Add(Damage_Fire, Attributes_Resistance_Fire);

	/*
	 * Map of Damage Types to Debuffs
	 */
	DamageTypesToDebuffs.Add(Damage_Arcane, Debuff_Arcane);
	DamageTypesToDebuffs.Add(Damage_Lightning, Debuff_Stun);
	DamageTypesToDebuffs.Add(Damage_Physical, Debuff_Physical);
	DamageTypesToDebuffs.Add(Damage_Fire, Debuff_Burn);
	
	/*
	 * 各种类型的前缀
	 */
	Attribute_Primary = FGameplayTag::RequestGameplayTag(FName("Attribute.Primary"));
	Abilities = FGameplayTag::RequestGameplayTag(FName("Abilities"));

	/**
	 *	物品类型
	 */
	InitTagVariable(Inventory_Category_Equipment, "Inventory.Category.Equipment", TEXT("装备"));
	InitTagVariable(Inventory_Category_Consumable, "Inventory.Category.Consumable", TEXT("消耗品"));
	InitTagVariable(Inventory_Category_Common, "Inventory.Category.Common", TEXT("普通物品"));
	InitTagVariable(Inventory_Category_Equipment_Weapon, "Inventory.Category.Equipment.Weapon", TEXT("武器"));
	InitTagVariable(Inventory_Category_Equipment_HeadArmor, "Inventory.Category.Equipment.HeadArmor", TEXT("头部防具，如帽子、头盔"));
	InitTagVariable(Inventory_Category_Equipment_UpperBodyArmor, "Inventory.Category.Equipment.UpperBodyArmor", TEXT("上身防具，如布衣、盔甲"));
	InitTagVariable(Inventory_Category_Equipment_LowerBodyArmor, "Inventory.Category.Equipment.LowerBodyArmor", TEXT("下身防具，如裤子、裙子"));
	InitTagVariable(Inventory_Category_Equipment_UpperLimbArmor, "Inventory.Category.Equipment.UpperLimbArmor", TEXT("上肢防具，如护腕、手套"));
	InitTagVariable(Inventory_Category_Equipment_LowerLimbArmor, "Inventory.Category.Equipment.LowerLimbArmor", TEXT("下肢防具，如护膝、鞋子"));
	InitTagVariable(Inventory_Category_Equipment_Accessory, "Inventory.Category.Equipment.Accessory", TEXT("饰品，如项链、戒子"));
	
	/*
	 * 广播初始化完毕
	 */
	OnInitedDelegate.Broadcast();
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"), FString("Player_Block_CursorTrace"));
}
