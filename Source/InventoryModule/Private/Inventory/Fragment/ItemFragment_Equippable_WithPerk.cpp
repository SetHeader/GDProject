// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Fragment/ItemFragment_Equippable_WithPerk.h"

#include "AbilitySystemComponent.h"
#include "GDLog.h"
#include "Inventory/InventoryItemInstance.h"


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

bool UItemFragment_Equippable_WithPerk::GiveToAbilitySystem(UAbilitySystemComponent* ASC,
                                                            FAbilitySet_GrantHandles* OutHandleStorage,
                                                            UInventoryItemInstance* SourceObject) const {

	if (Super::GiveToAbilitySystem(ASC, OutHandleStorage, SourceObject))
		if (IsValid(EquipmentPerk.Get()) && IsValid(SourceObject)) {
			FGameplayEffectSpecHandle EffectSpecHandle = MakeEquipmentPerkEffectSpec(ASC, PerkContainer);
			const FActiveGameplayEffectHandle GameplayEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
			OutHandleStorage->AddGameplayEffectHandle(GameplayEffectHandle);
		}

	return true;
}

void UItemFragment_Equippable_WithPerk::OnInstancedCreated(UInventoryItemInstance* Instance) {
	Super::OnInstancedCreated(Instance);

	TSubclassOf<UItemPerk> Perk = EquipmentPerk.LoadSynchronous();
	if (Perk) {
		PerkContainer = Perk.GetDefaultObject()->GenerateItemPerk();
	}
}

FGameplayEffectSpecHandle UItemFragment_Equippable_WithPerk::MakeEquipmentPerkEffectSpec(UAbilitySystemComponent* ASC,
                                                                                         const FItemPerkContainer& OutPerkContainer) const {
	if (!IsValid(ASC))
		return FGameplayEffectSpecHandle();
	
	TObjectPtr<UGameplayEffect> Effect = NewObject<UGameplayEffect>();
	
	Effect->DurationPolicy = EGameplayEffectDurationType::Infinite;
	
	FGameplayModifierInfo MainAttributeModifier;
	MainAttributeModifier.Attribute = OutPerkContainer.MainAttributeDefine.Attribute;
	MainAttributeModifier.ModifierOp = OutPerkContainer.MainAttributeDefine.ModifierOp;
	MainAttributeModifier.ModifierMagnitude = FScalableFloat(OutPerkContainer.MainAttributeDefine.Value);
	
	Effect->Modifiers.Add(MoveTemp(MainAttributeModifier));

	for (const auto& AttributeDefine : OutPerkContainer.SecondaryAttributeDefines) {
		FGameplayModifierInfo AttributeModifier;
		AttributeModifier.Attribute = AttributeDefine.Attribute;
		AttributeModifier.ModifierOp = AttributeDefine.ModifierOp;
		AttributeModifier.ModifierMagnitude = FScalableFloat(AttributeDefine.Value);
	
		Effect->Modifiers.Add(MoveTemp(AttributeModifier));
	}
	
	FGameplayEffectSpec* EffectSpec = new FGameplayEffectSpec(Effect, ASC->MakeEffectContext(), 1);
	return FGameplayEffectSpecHandle(EffectSpec);;
}
