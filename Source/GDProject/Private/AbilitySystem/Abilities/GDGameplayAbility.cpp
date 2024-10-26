// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GDGameplayAbility.h"

#include "AbilitySystem/AttributeSets/GDAttributeSet.h"

FString UGDGameplayAbility::GetDescription(int32 InLevel)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", InLevel);
}

FString UGDGameplayAbility::GetNextLevelDescription(int32 InLevel)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), InLevel);
}

FString UGDGameplayAbility::GetLockedDescription()
{
	return FString::Printf(TEXT("<Default>Spell Locked</>"));
}

float UGDGameplayAbility::GetManaCost(float InLevel) const
{
	UGameplayEffect* CostEffect = GetCostGameplayEffect();
	if (CostEffect)
	{
		FGameplayAttribute ManaAttribute = UGDAttributeSet::GetManaAttribute();
		for (const FGameplayModifierInfo& Modifier : CostEffect->Modifiers)
		{
			if (ManaAttribute == Modifier.Attribute)
			{
				float CostValue = 0;
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, CostValue);
				return CostValue;
			}
		} 
	}
	
	return 0.f;
}

float UGDGameplayAbility::GetCooldown(float InLevel) const
{
	float CooldownValue = 0;
	
	if (UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, CooldownValue);
	}
	
	return CooldownValue;
}
