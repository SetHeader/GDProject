// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GDPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "GDLOG.h"

void UGDPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                        const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	GDLOG(TEXT("GDPassiveAbility"), TEXT("Activate %s"), *GetName());
	if (auto GDASC = Cast<UGDAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		if (!GDASC->OnPassiveAbilityDeactivated.IsBoundToObject(this))
		{
			GDASC->OnPassiveAbilityDeactivated.AddUObject(this, &UGDPassiveAbility::ReceiveDeactivate);
		}
	}
}

void UGDPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		GDLOG(TEXT("GDPassiveAbility"), TEXT("EndAbility %s"), *GetName());
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
