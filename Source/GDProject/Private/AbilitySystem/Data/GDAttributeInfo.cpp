// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/GDAttributeInfo.h"

const FAttributeInfo& UGDAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAttributeInfo& Info : AttributeInfos) {
		if (Info.AttributeTag.MatchesTagExact(AttributeTag)) {
			return Info;
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("没找到属性[%s]"), *AttributeTag.ToString());
	return NoneAttribute;
}
