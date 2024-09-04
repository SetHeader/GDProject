// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/GDOverlayWidgetController.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "UI/Widget/GDUserWidget.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "GameplayTagContainer.h"

void UGDOverlayWidgetController::BindCallbacksToDependencies() const
{
	UGDAttributeSet* GDAS = CastChecked<UGDAttributeSet>(AttributeSet);
	check(AbilitySystemComponent);

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
	}
}

void UGDOverlayWidgetController::BroadcastInitialValues() const
{
	UGDAttributeSet* GDAS = CastChecked<UGDAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(GDAS->GetHealth());
	OnMaxHealthChanged.Broadcast(GDAS->GetMaxHealth());
	OnManaChanged.Broadcast(GDAS->GetMana());
	OnMaxManaChanged.Broadcast(GDAS->GetMaxMana());
	OnStaminaChanged.Broadcast(GDAS->GetStamina());
	OnMaxStaminChanged.Broadcast(GDAS->GetMaxStamina());
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
