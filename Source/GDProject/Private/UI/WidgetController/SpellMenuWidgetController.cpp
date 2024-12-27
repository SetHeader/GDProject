// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/GDGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "GDProject/GDProject.h"
#include "Player/GDPlayerState.h"

void USpellMenuWidgetController::BindCallbacksToDependencies()
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
		
		if (GDASC->IsASCInited())
		{
			OnInitializeStartupAbilities(GDASC);
		}
		else
		{
			GDASC->OnAbilitiesGiven.AddUObject(this, &USpellMenuWidgetController::OnInitializeStartupAbilities);
		}
	}

	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);
	GDPS->OnSpellPointsChangedDelegate.AddLambda([&](const int32 SpellPoints)
	{
		OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
	});
}

void USpellMenuWidgetController::BroadcastInitialValues()
{
	Super::BroadcastInitialValues();
	if (!AbilityInfo) {
		GDLog::ErrorStatic(TEXT("USpellMenuWidgetController"), TEXT("AbilityInfo is nullptr"));
		return;
	}
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);
	OnSpellPointsChangedDelegate.Broadcast(GDPS->GetSpellPoints());
	for (const FGDAbilityInfo& Info : AbilityInfo->AbilityInfos)
	{
		OnAbilityInfoChangedDelegate.Broadcast(Info);
	}
}

void USpellMenuWidgetController::OnInitializeStartupAbilities(UGDAbilitySystemComponent* GDASC) {
	AbilityInfo = GDASC->AbilityInfo;
	BroadcastInitialValues();
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
	if (!AbilityInfo) {
		GDLog::ErrorStatic(TEXT("USpellMenuWidgetController"), TEXT("AbilityInfo is nullptr"));
		return FGDAbilityInfo();
	}
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

FString USpellMenuWidgetController::GetDescription(FGameplayTag AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = CastChecked<UGDAbilitySystemComponent>(AbilitySystemComponent)->GetSpecFromAbilityTag(AbilityTag))
	{
		return CastChecked<UGDGameplayAbility>(AbilitySpec->Ability)->GetDescription(AbilitySpec->Level);
	}

	return TEXT("");
}

FString USpellMenuWidgetController::GetNextLevelDescription(FGameplayTag AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = CastChecked<UGDAbilitySystemComponent>(AbilitySystemComponent)->GetSpecFromAbilityTag(AbilityTag))
	{
		return CastChecked<UGDGameplayAbility>(AbilitySpec->Ability)->GetNextLevelDescription(AbilitySpec->Level);
	}

	return TEXT("");
}

FString USpellMenuWidgetController::GetLockedDescription(FGameplayTag AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = CastChecked<UGDAbilitySystemComponent>(AbilitySystemComponent)->GetSpecFromAbilityTag(AbilityTag))
	{
		return CastChecked<UGDGameplayAbility>(AbilitySpec->Ability)->GetLockedDescription();
	}

	return TEXT("");
}
