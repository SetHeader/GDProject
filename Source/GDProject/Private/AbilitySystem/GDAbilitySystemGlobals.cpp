// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDAbilitySystemGlobals.h"

#include "AbilitySystem/GDAbilityTypes.h"

FGameplayEffectContext* UGDAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGDGameplayEffectContext();
}
