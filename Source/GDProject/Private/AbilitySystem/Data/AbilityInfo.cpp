// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "GameplayAbilitySpec.h"
#include "GDGameplayTags.h"
#include "GDProject/GDLog.h"

FGameplayAbilitySpec FGDAbilityInfo::CreateAbilitySpec(int32 AbilityLevel) const
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, AbilityLevel);
	AbilitySpec.DynamicAbilityTags.AddTag(StatusTag);
	AbilitySpec.DynamicAbilityTags.AddTag(AbilityType);
	if (InputTag.IsValid())
	{
		AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
	}
	return AbilitySpec;
}

FGDAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FGDAbilityInfo& AbilityInfo : AbilityInfos)
	{
		if (AbilityInfo.AbilityTag == Tag)
		{
			return AbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogGD, Error, TEXT("AbilityInfo\t Not Found Ability With Tag: %s"), *Tag.ToString())
	}

	return FGDAbilityInfo();
}

FGDAbilityInfo* UAbilityInfo::FindAbilityInfoPtrForTag(const FGameplayTag& Tag, bool bLogNotFound)
{
	for (FGDAbilityInfo& AbilityInfo : AbilityInfos)
	{
		if (AbilityInfo.AbilityTag == Tag)
		{
			return &AbilityInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogGD, Error, TEXT("AbilityInfo\t Not Found Ability With Tag: %s"), *Tag.ToString())
	}

	return nullptr;
}
