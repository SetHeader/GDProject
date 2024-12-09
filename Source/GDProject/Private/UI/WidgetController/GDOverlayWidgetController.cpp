// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/GDOverlayWidgetController.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GDLog.h"
#include "Player/GDPlayerState.h"

void UGDOverlayWidgetController::BindCallbacksToDependencies() const
{
	UGDAttributeSet* GDAS = CastChecked<UGDAttributeSet>(AttributeSet);
	check(AbilitySystemComponent);

	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);
	GDPS->OnXPChangedDelegate.AddUObject(this, &UGDOverlayWidgetController::OnXPChanged);
	GDPS->OnLevelChangedDelegate.AddLambda([this](const int32 NewLevel, bool bLevelUp)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
	});
	GDPS->OnAttributePointsChangedDelegate.AddLambda([this](const int32 NewPoints)
	{
		OnAttributePointsChangedDelegate.Broadcast(NewPoints);
	});
	GDPS->OnSpellPointsChangedDelegate.AddLambda([this](const int32 NewPoints)
	{
		OnSpellPointsChangedDelegate.Broadcast(NewPoints);
	});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GDAS->GetHealthAttribute()).AddUObject(this, &UGDOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GDAS->GetMaxHealthAttribute()).AddUObject(this, &UGDOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GDAS->GetManaAttribute()).AddUObject(this, &UGDOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GDAS->GetMaxManaAttribute()).AddUObject(this, &UGDOverlayWidgetController::MaxManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GDAS->GetStaminaAttribute()).AddUObject(this, &UGDOverlayWidgetController::StaminaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GDAS->GetMaxStaminaAttribute()).AddUObject(this, &UGDOverlayWidgetController::MaxStaminaChanged);

	check(MessageWidgetDataTable);

	if (UGDAbilitySystemComponent* GDASC = Cast<UGDAbilitySystemComponent>(AbilitySystemComponent)) {
		// 在客户端绑定，且只会在客户端通知
		GDASC->OnEffectTagApplied.AddLambda([this](const FGameplayTagContainer& TagContainer) {

			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			for (const FGameplayTag& Tag : TagContainer.GetGameplayTagArray()) {
				if (Tag.MatchesTag(MessageTag)) {
					FUIWidgetRow* Row = MessageWidgetDataTable->FindRow<FUIWidgetRow>(Tag.GetTagName(), TEXT(""));
					OnMessageWidgetRow.Broadcast(*Row);
				}
			}
		});
		
		if (GDASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(GDASC);
		}
		else
		{
			GDASC->OnAbilitiesGiven.AddUObject(this, &UGDOverlayWidgetController::OnInitializeStartupAbilities);
		}

		GDASC->OnAbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
		{
			BroadcastAbilityInfos();
		});
	}
}

void UGDOverlayWidgetController::BroadcastInitialValues() const
{
	UGDAttributeSet* GDAS = CastChecked<UGDAttributeSet>(AttributeSet);
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);
	
	OnPlayerLevelChangedDelegate.Broadcast(GDPS->GetPlayerLevel(), false);
	OnHealthChanged.Broadcast(GDAS->GetHealth());
	OnMaxHealthChanged.Broadcast(GDAS->GetMaxHealth());
	OnManaChanged.Broadcast(GDAS->GetMana());
	OnMaxManaChanged.Broadcast(GDAS->GetMaxMana());
	OnStaminaChanged.Broadcast(GDAS->GetStamina());
	OnMaxStaminChanged.Broadcast(GDAS->GetMaxStamina());

	BroadcastAbilityInfos();
}

void UGDOverlayWidgetController::OnInitializeStartupAbilities(UGDAbilitySystemComponent* ASC) const
{
	if (!ASC->bStartupAbilitiesGiven)
	{
		return;
	}

	BroadcastAbilityInfos();
}

void UGDOverlayWidgetController::BroadcastAbilityInfos() const
{
	for (const FGDAbilityInfo& Info : AbilityInfoAsset->AbilityInfos)
	{
		OnAbilityInfoDelegate.Broadcast(Info);
	}
}


void UGDOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& ChangedData) const
{
	OnHealthChanged.Broadcast(ChangedData.NewValue);
}

void UGDOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& ChangedData) const
{
	OnMaxHealthChanged.Broadcast(ChangedData.NewValue);
}

void UGDOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& ChangedData) const
{
	OnManaChanged.Broadcast(ChangedData.NewValue);
}

void UGDOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& ChangedData) const
{
	OnMaxManaChanged.Broadcast(ChangedData.NewValue);
}

void UGDOverlayWidgetController::StaminaChanged(const FOnAttributeChangeData& ChangedData) const
{
	OnStaminaChanged.Broadcast(ChangedData.NewValue);
}

void UGDOverlayWidgetController::MaxStaminaChanged(const FOnAttributeChangeData& ChangedData) const
{
	OnMaxStaminChanged.Broadcast(ChangedData.NewValue);
}

void UGDOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(PlayerState);
	
	if (!GDPS->LevelUpInfo)
	{
		GDLOG_E(TEXT("UGDOverlayWidgetController"), TEXT("%hs\t Not Set LevelUpInfo"), __FUNCTION__);
		OnXPPercentChangedDelegate.Broadcast(0.f);
		return;
	}
	
	int32 Level = GDPS->LevelUpInfo->FindLevelForXP(NewXP);
	// 满级后就 通知 满进度条
	if (Level >= GDPS->LevelUpInfo->LevelUpInfos.Num() - 1)
	{
		OnXPPercentChangedDelegate.Broadcast(1.f);
		return;
	}
	
	// 当前级 经验条上限
	int32 XPCurrentLevel = GDPS->LevelUpInfo->LevelUpInfos[Level].LevelUpRequirement;
	// 上一级 经验条上限
	int32 XPPreviousLevel = GDPS->LevelUpInfo->LevelUpInfos[Level - 1].LevelUpRequirement;
	
	OnXPPercentChangedDelegate.Broadcast(static_cast<float>(NewXP - XPPreviousLevel) / (XPCurrentLevel - XPPreviousLevel));
}

FGDAbilityInfo UGDOverlayWidgetController::FindAbilityInfoByInputTag(FGameplayTag InputTag) const
{
	for (FGDAbilityInfo& AbilityInfo : AbilityInfoAsset->AbilityInfos)
	{
		if (AbilityInfo.InputTag == InputTag)
		{
			return AbilityInfo;
		}
	}
	return FGDAbilityInfo();
}
