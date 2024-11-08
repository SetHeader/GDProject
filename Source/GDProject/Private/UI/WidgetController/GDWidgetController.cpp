// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/GDWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"

UGDAbilitySystemComponent* UGDWidgetController::GetGDASC() const
{
	return Cast<UGDAbilitySystemComponent>(AbilitySystemComponent);
}
