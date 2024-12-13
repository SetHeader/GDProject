// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Fragment/ItemFragment_Available.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Inventory/InventoryItemInstance.h"

bool UItemFragment_Available::UseItem(const FUseItemParams& Params) {
	if (IsValid(Params.TargetASC)) 
		return GiveToAbilitySystem(Params.TargetASC, nullptr);
	
	return false;
}

bool UItemFragment_Available::GiveToAbilitySystem(UAbilitySystemComponent* ASC,
                                                  FAbilitySet_GrantHandles* OutHandleStorage) const {
	if (!IsValid(ASC) || !OwnerItemInstance.IsValid())
		return false;
	
	for (const auto& AbilityToGrant : EquipmentAbilitySet.GrantAbilities) {
		if (AbilityToGrant.Ability) {
			FGameplayAbilitySpec AbilitySpec(AbilityToGrant.Ability, AbilityToGrant.AbilityLevel);
			AbilitySpec.SourceObject = OwnerItemInstance;

			FGameplayAbilitySpecHandle AbilitySpecHandle;
			
			if (AbilityToGrant.bActivateOnce) 
				AbilitySpecHandle = ASC->GiveAbilityAndActivateOnce(AbilitySpec);
			else
				AbilitySpecHandle = ASC->GiveAbility(AbilitySpec);

			if (OutHandleStorage)
				OutHandleStorage->AddAbilitySpecHandle(AbilitySpecHandle);
		}
	}

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(OwnerItemInstance.Get());
	for (const auto& EffectToGrant : EquipmentAbilitySet.GrantEffects) {
		if (EffectToGrant.GameplayEffect) {
			FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectToGrant.GameplayEffect, EffectToGrant.EffectLevel, EffectContextHandle);
			FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
			if (OutHandleStorage)
				OutHandleStorage->AddGameplayEffectHandle(ActiveEffectHandle);
		}
	}
	
	return true;
}
