// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/GDAttributeWidgetController.h"
#include "AbilitySystem/Data/GDAttributeInfo.h"
#include "AbilitySystem/AttributeSets/GDAttributeSetBase.h"
#include "GameplayTagContainer.h"

void UGDAttributeWidgetController::BroadcastInitialValues() const
{
	if (IsValid(AttributesInfo)) {
		UGDAttributeSetBase* AS = CastChecked<UGDAttributeSetBase>(AttributeSet);
		
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
	const UGDAttributeSetBase* AS = Cast<UGDAttributeSetBase>(AttributeSet);
	
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