// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Fragment/ItemFragment_Equipment.h"

#include "AbilitySystemComponent.h"
#include "GDLog.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/Fragment/ItemFragment_EquipmentInstance.h"


FItemPerkContainer UItemPerk::GenerateItemPerk_Implementation() {
	return FItemPerkContainer();
}

UDataTable* UItemPerk::GetItemAttributeDataTable() const {
	UDataTable* DataTable = GetDefault<UInventorySettings>()->EquipmentAttributeDefineTable.LoadSynchronous();
	if (!IsValid(DataTable)) {
		GDLOG_E(TEXT("UItemPerk"), TEXT("没有配置 EquipmentAttributeDefineTable"));
		return nullptr;
	}
	return DataTable;
}

FItemAttributeRowDefine* UItemPerk::FindItemAttributeRowDefine(FName RowName) const {
	UDataTable* DataTable = GetItemAttributeDataTable();
	if (DataTable) {
		return DataTable->FindRow<FItemAttributeRowDefine>(RowName, RowName.ToString(), true);
	}
	return nullptr;
}

FItemPerkContainer UItemPerk_RandomEquipAttribute::GenerateItemPerk_Implementation() {
	FItemPerkContainer PerkContainer;
	UDataTable* DataTable = GetItemAttributeDataTable();
	if (!DataTable) 
		return PerkContainer;
	
	TArray<FName> RowNames = DataTable->GetRowNames();
	if (RowNames.IsEmpty()) 
		return PerkContainer;

	if (const FItemAttributeRowDefine* AttributeDefine = FindItemAttributeRowDefine(RowNames[FMath::RandRange(0, RowNames.Num() - 1)])) {
		PerkContainer.MainAttributeDefine.Attribute = AttributeDefine->Attribute;
		PerkContainer.MainAttributeDefine.ModifierOp = AttributeDefine->ModifierOp;
		PerkContainer.MainAttributeDefine.Value = AttributeDefine->Magnitude.GetValueAtLevel(1.0);
	}

	for (int i = 0; i < 3; ++i) {
		if (const FItemAttributeRowDefine* AttributeDefine = FindItemAttributeRowDefine(RowNames[FMath::RandRange(0, RowNames.Num() - 1)])) {
			FItemAttributeDefine SecondaryAttributeDefine;
			SecondaryAttributeDefine.Attribute = AttributeDefine->Attribute;
			SecondaryAttributeDefine.ModifierOp = AttributeDefine->ModifierOp;
			SecondaryAttributeDefine.Value = AttributeDefine->Magnitude.GetValueAtLevel(1.0);
			PerkContainer.SecondaryAttributeDefines.Add(MoveTemp(SecondaryAttributeDefine));
		}
	}

	return PerkContainer;
}

bool UItemFragment_Equipment::GiveToAbilitySystem(UAbilitySystemComponent* ASC,
                                                  FAbilitySet_GrantHandles* OutHandleStorage,
                                                  UInventoryItemInstance* SourceObject) const {
	if (!IsValid(ASC) || !OutHandleStorage || !IsValid(SourceObject)) 
		return false;

	for (const auto& AbilityToGrant : EquipmentAbilitySet.GrantAbilities) {
		if (AbilityToGrant.Ability) {
			FGameplayAbilitySpec AbilitySpec(AbilityToGrant.Ability, AbilityToGrant.AbilityLevel);
			AbilitySpec.SourceObject = SourceObject;

			FGameplayAbilitySpecHandle AbilitySpecHandle;
			
			if (AbilityToGrant.bActivateOnce) 
				AbilitySpecHandle = ASC->GiveAbilityAndActivateOnce(AbilitySpec);
			else
				AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);
			
			OutHandleStorage->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceObject);
	for (const auto& EffectToGrant : EquipmentAbilitySet.GrantEffects) {
		if (EffectToGrant.GameplayEffect) {
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectToGrant.GameplayEffect, EffectToGrant.EffectLevel, EffectContextHandle);
			FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
			OutHandleStorage->AddGameplayEffectHandle(ActiveEffectHandle);
		}
	}

	if (IsValid(EquipmentPerk.Get()) && IsValid(SourceObject)) {
		if (const auto* EquipmentInstanceFragment = SourceObject->FindFragmentByClass<UItemFragment_EquipmentInstance>()) {
			FGameplayEffectSpecHandle EffectSpecHandle = MakeEquipmentPerkEffectSpec(ASC, EquipmentInstanceFragment->PerkContainer);
			const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
			OutHandleStorage->AddGameplayEffectHandle(GameplayEffectHandle);
		}
	}
	
	return true;
}

FGameplayEffectSpecHandle UItemFragment_Equipment::MakeEquipmentPerkEffectSpec(UAbilitySystemComponent* ASC,
	const FItemPerkContainer& Perk) const {
	if (!IsValid(ASC))
		return FGameplayEffectSpecHandle();
	
	TObjectPtr<UGameplayEffect> Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName("EquipmentEffect"));
	
	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	FGameplayModifierInfo MainAttributeModifier;
	MainAttributeModifier.Attribute = Perk.MainAttributeDefine.Attribute;
	MainAttributeModifier.ModifierOp = Perk.MainAttributeDefine.ModifierOp;
	MainAttributeModifier.ModifierMagnitude = FScalableFloat(Perk.MainAttributeDefine.Value);
	
	Effect->Modifiers.Add(MoveTemp(MainAttributeModifier));

	for (const auto& AttributeDefine : Perk.SecondaryAttributeDefines) {
		FGameplayModifierInfo AttributeModifier;
		AttributeModifier.Attribute = Perk.MainAttributeDefine.Attribute;
		AttributeModifier.ModifierOp = Perk.MainAttributeDefine.ModifierOp;
		AttributeModifier.ModifierMagnitude = FScalableFloat(Perk.MainAttributeDefine.Value);
	
		Effect->Modifiers.Add(MoveTemp(AttributeModifier));
	}
	
	FGameplayEffectSpec* EffectSpec = new FGameplayEffectSpec(Effect, ASC->MakeEffectContext(), 1);
	return FGameplayEffectSpecHandle(EffectSpec);;
}
