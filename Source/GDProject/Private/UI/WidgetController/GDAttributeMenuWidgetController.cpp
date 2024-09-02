#include "UI/WidgetController/GDAttributeMenuWidgetController.h"
#include "AbilitySystem/Data/GDAttributeInfo.h"
#include "AbilitySystem/AttributeSets/GDAttributeSetBase.h"
#include "GameplayTagContainer.h"
#include "GDGameplayTags.h"
#include "Tools/FArrayTool.h"
#include "Tools/FReflectTool.h"

void UGDAttributeMenuWidgetController::BroadcastInitialValues() const
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
		
		// TArray<FProperty*> AttributeNames;
		// FReflectTool::GetClassPropertysByPropType(UGDAttributeSetBase::StaticClass(), FString("FGameplayAttributeData"), AttributeNames);
		//
		// for (FAttributeInfo& Info : AttributesInfo->AttributeInfos) {
		// 	for (const FProperty* Property : AttributeNames)
		// 	{
		// 		if (!Property)
		// 		{
		// 			continue;
		// 		}
		// 		// 属性名，如Strength、Intelligence等
		// 		FString AttributeName = Property->GetName();
		//
		// 		// 反射拿到属性对应的Tag
		// 		const FGameplayTag* Tag = FReflectTool::GetPropertyValue<FGameplayTag>(
		// 			FReflectTool::GetStructProperty<FGDGameplayTags>(AttributeName, true),
		// 			&FGDGameplayTags::Get());
		// 		
		// 		if (Tag && Info.AttributeTag.MatchesTagExact(*Tag))
		// 		{
		// 			// 反射获取属性值
		// 			// 不用反射调用 GetStrength 函数是因为该函数没有加UFUNCTION()标记，反射不到
		// 			const FGameplayAttributeData* AttributeData = FReflectTool::GetClassPropertyValue<FGameplayAttributeData>(AS, AttributeName);
		// 			
		// 			if (AttributeData)
		// 			{
		// 				float Value = AttributeData->GetCurrentValue();
		// 				Info.AttributeValue = Value;
		// 				OnAttributeChanged.Broadcast(Info);
		// 			}
		// 		}
		// 	}
		// }
	}
}

void UGDAttributeMenuWidgetController::BindCallbacksToDependencies() const
{
	const UGDAttributeSetBase* AS = Cast<UGDAttributeSetBase>(AttributeSet);
	

	for (FAttributeInfo& Info : AttributesInfo->AttributeInfos)
	{
		if (const TStaticFuncPtr<FGameplayAttribute()>* Func = AS->Tag2AttributeFuncMap.Find(Info.AttributeTag))
		{
			FGameplayAttribute Attribute = (*Func)();
			
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
				.AddUObject(this, &UGDAttributeMenuWidgetController::BroadcastAttributeChanged, Info.AttributeTag);
		}
	}
}

void UGDAttributeMenuWidgetController::BroadcastAttributeChanged(const FOnAttributeChangeData& ChangedData, FGameplayTag AttributeTag) const
{
	FAttributeInfo Info = AttributesInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = ChangedData.NewValue;
	OnAttributeChanged.Broadcast(Info);
}

void UGDAttributeMenuWidgetController::AddValue(FGameplayTag Tag)
{
	UGDAttributeSetBase* AS = Cast<UGDAttributeSetBase>(AttributeSet);
	if (!AS)
	{
		return;
	}
	
	if (Tag == FGDGameplayTags::Get().Attribute_Primary_Strength)
	{
		AS->SetHealth(AS->GetHealth() + 1);
	} else if (Tag == FGDGameplayTags::Get().Attribute_Primary_Intelligence)
	{
		AS->SetIntelligence(AS->GetIntelligence() + 1);
	} else if (Tag == FGDGameplayTags::Get().Attribute_Primary_Resilience)
	{
		AS->SetResilience(AS->GetResilience() + 1);
	} else if (Tag == FGDGameplayTags::Get().Attribute_Primary_Vigor)
	{
		AS->SetVigor(AS->GetVigor() + 1);
	}
}
