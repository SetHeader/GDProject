// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/GDAttributeWidgetController.h"
#include "AbilitySystem/Data/GDAttributeInfo.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "GameplayTagContainer.h"

void UGDAttributeWidgetController::BroadcastInitialValues() const
{
	if (IsValid(AttributesInfo)) {
		UGDAttributeSet* AS = CastChecked<UGDAttributeSet>(AttributeSet);
		
		for (FAttributeInfo& Info : AttributesInfo->AttributeInfos)
		{
			if (TStaticFuncPtr<FGameplayAttribute()>* Func = AS->Tag2AttributeFuncMap.Find(Info.AttributeTag))
			{
				FGameplayAttribute Attribute = (*Func)();
				Info.AttributeValue = Attribute.GetNumericValue(AS);
				OnAttributeChanged.Broadcast(Info);
			}
		}
	}
}

void UGDAttributeWidgetController::BindCallbacksToDependencies() const
{
	const UGDAttributeSet* AS = Cast<UGDAttributeSet>(AttributeSet);
	
	for (FAttributeInfo& Info : AttributesInfo->AttributeInfos)
	{
		if (const TStaticFuncPtr<FGameplayAttribute()>* Func = AS->Tag2AttributeFuncMap.Find(Info.AttributeTag))
		{
			FGameplayAttribute Attribute = (*Func)();
			
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
				.AddUObject(this, &ThisClass::BroadcastAttributeChanged, Info.AttributeTag);
		}
	}
}

void UGDAttributeWidgetController::BroadcastAttributeChanged(const FOnAttributeChangeData& ChangedData, FGameplayTag AttributeTag) const
{
	FAttributeInfo Info = AttributesInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = ChangedData.NewValue;
	OnAttributeChanged.Broadcast(Info);
}