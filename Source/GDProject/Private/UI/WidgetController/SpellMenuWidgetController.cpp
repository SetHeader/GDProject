// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void USpellMenuWidgetController::BroadcastInitialValues() const
{
	Super::BroadcastInitialValues();
}

void USpellMenuWidgetController::BindCallbacksToDependencies() const
{
	UGDAbilitySystemComponent* GDASC = GetGDASC();
	if (GDASC) {
		GDASC->OnAbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			if (AbilityInfo)
			{
				FGDAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				OnAbilityInfoChangedDelegate.Broadcast(Info);
			}
		});
	}
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
