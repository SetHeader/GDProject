// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "GDGameplayTags.h"
#include "GDProject/GDLogChannels.h"

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
