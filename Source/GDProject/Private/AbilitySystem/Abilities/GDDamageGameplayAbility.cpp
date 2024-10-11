// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GDDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UGDDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	check(TargetActor);
	
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	
	const float DamageValue = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, DamageValue);
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("GDDamageGameplayAbility\t TargetActor Is Not a ASC. [%s]"), *TargetActor->GetName());
	}
}
