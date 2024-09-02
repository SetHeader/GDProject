// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GDHUD.h"

UGDOverlayWidgetController* UGDAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AGDHUD* GDHUD = Cast<AGDHUD>(PC->GetHUD()))
		{
			return Cast<UGDOverlayWidgetController>(GDHUD->GetOverlayWidgetController());
		}
	}
	return nullptr;
}

UGDAttributeMenuWidgetController* UGDAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AGDHUD* GDHUD = Cast<AGDHUD>(PC->GetHUD()))
		{
			return Cast<UGDAttributeMenuWidgetController>(GDHUD->GetAttributeMenuWidgetController());
		}
	}
	return nullptr;
}
