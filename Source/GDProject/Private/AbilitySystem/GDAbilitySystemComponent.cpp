// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDAbilitySystemComponent.h"
#include <AbilitySystemComponent.h>

#include "AbilitySystem/Abilities/GDGameplayAbility.h"


void UGDAbilitySystemComponent::OnAbilityActorInfoSet()
{
	// 在服务端和客户端都会绑定，但只会在服务端回调。但回调方法ClientEffectApplied是个rpc方法，会在客户端执行。
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UGDAbilitySystemComponent::ClientEffectApplied);
}

void UGDAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : SetupAbilities)
	{
		FGameplayAbilitySpec Ability = FGameplayAbilitySpec(AbilityClass, 1);
		if (UGDGameplayAbility* GDAbility = Cast<UGDGameplayAbility>(Ability.Ability))
		{
			Ability.DynamicAbilityTags.AddTag(GDAbility->SetupTag);
		}
		GiveAbility(Ability);
	}
}

void UGDAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	GESpec.GetAllAssetTags(TagContainer);
	if (!TagContainer.IsEmpty()) {
		OnEffectTagApplied.Broadcast(TagContainer);
	}
} 


void UGDAbilitySystemComponent::OnAbilityInputPressed(FGameplayTag InputTag)
{	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UGDAbilitySystemComponent::OnAbilityInputReleased(FGameplayTag InputTag)
{	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UGDAbilitySystemComponent::OnAbilityInputHeld(FGameplayTag InputTag)
{	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (!AbilitySpec.IsActive())
			{
				AbilitySpecInputPressed(AbilitySpec);
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}