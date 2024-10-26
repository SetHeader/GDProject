// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/GDPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues() const
{
	Super::BroadcastInitialValues();

	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);
	OnSpellPointsChangedDelegate.Broadcast(GDPS->GetSpellPoints());

	for (const FGDAbilityInfo& Info : AbilityInfo->AbilityInfos)
	{
		OnAbilityInfoChangedDelegate.Broadcast(Info);
	} 
}

void USpellMenuWidgetController::BindCallbacksToDependencies() const
{
	UGDAbilitySystemComponent* GDASC = GetGDASC();
	if (GDASC) {
		GDASC->OnAbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			if (AbilityInfo)
			{
				const FGDAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				OnAbilityInfoChangedDelegate.Broadcast(Info);
			}
		});
	}

	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);
	GDPS->OnSpellPointsChangedDelegate.AddLambda([&](const int32 SpellPoints)
	{
		OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
	});
}

void USpellMenuWidgetController::SelectAbility(FGameplayTag AbilityTag)
{
	if (AbilityTag == SelectedAbilityTag)
	{
		return;
	}

	SelectedAbilityTag = AbilityTag;
	OnAbilitySelected.Broadcast(SelectedAbilityTag);
}

FGDAbilityInfo USpellMenuWidgetController::FindAbilityInfoForTag(const FGameplayTag& AbilityTag) const
{
	return AbilityInfo->FindAbilityInfoForTag(AbilityTag);
}

void USpellMenuWidgetController::UpgradeAbility(FGameplayTag AbilityTag) const
{
	CastChecked<UGDAbilitySystemComponent>(AbilitySystemComponent)->ServerUpgradeAbility(AbilityTag);
}

void USpellMenuWidgetController::EquipAbility(FGameplayTag AbilityTag, FGameplayTag InputTag) const
{
	CastChecked<UGDAbilitySystemComponent>(AbilitySystemComponent)->ServerEquipAbility(AbilityTag, InputTag);
}
