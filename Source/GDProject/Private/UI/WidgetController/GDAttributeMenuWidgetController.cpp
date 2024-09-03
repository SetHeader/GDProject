#include "UI/WidgetController/GDAttributeMenuWidgetController.h"

#include "AbilitySystem/AttributeSets/GDAttributeSetBase.h"
#include "GameplayTagContainer.h"
#include "GDGameplayTags.h"


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
