#include "UI/WidgetController/GDAttributeMenuWidgetController.h"

#include "GameplayTagContainer.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "Player/GDPlayerState.h"

void UGDAttributeMenuWidgetController::BroadcastInitialValues() const
{
	Super::BroadcastInitialValues();
	
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);
	OnAttributePointsChangedDelegate.Broadcast(GDPS->GetAttributePoints());
}

void UGDAttributeMenuWidgetController::BindCallbacksToDependencies() const
{
	Super::BindCallbacksToDependencies();
	
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);

	GDPS->OnAttributePointsChangedDelegate.AddLambda([this](const int32 Points)
	{
		OnAttributePointsChangedDelegate.Broadcast(Points);
	});
}

void UGDAttributeMenuWidgetController::UpgradeAttributePoint(FGameplayTag Tag)
{
	if (UGDAbilitySystemComponent* ASC = Cast<UGDAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->UpgradeAttributePoint(Tag);
	}
}
