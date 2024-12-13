// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Fragment/ItemFragment_Equippable.h"

#include "AbilitySystemComponent.h"
#include "Inventory/InventoryItemInstance.h"

bool UItemFragment_Equippable::GiveToAbilitySystem(UAbilitySystemComponent* ASC,
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
	
	return true;
}