// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDGameplayAbility.h"
#include "GDPassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDPassiveAbility : public UGDGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};
