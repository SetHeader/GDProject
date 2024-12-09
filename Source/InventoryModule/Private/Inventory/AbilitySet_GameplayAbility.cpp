#include "Inventory/AbilitySet_GameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"

void FAbilitySet_GrantHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle) {
	if (Handle.IsValid()) 
		AbilitySpecHandles.Add(Handle);
}

void FAbilitySet_GrantHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle) {
	if (Handle.IsValid())
		GameplayEffectHandles.Add(Handle);
}

void FAbilitySet_GrantHandles::TakeFromAbilitySystem(UAbilitySystemComponent* ASC) {
	if (IsValid(ASC)) {
		for (const auto& Handle : AbilitySpecHandles) 
			if (Handle.IsValid()) 
				ASC->ClearAbility(Handle);
		
		for (const auto& Handle : GameplayEffectHandles) 
			if (Handle.IsValid()) 
				ASC->RemoveActiveGameplayEffect(Handle);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
}