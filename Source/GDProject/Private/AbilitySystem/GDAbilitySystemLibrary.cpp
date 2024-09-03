// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDAbilitySystemLibrary.h"

#include "AbilitySystemComponent.h"
#include "GDGameModeBase.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
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

void UGDAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	checkf(WorldContextObject, TEXT("WorldContextObject is null"));
	checkf(ASC, TEXT("ASC is null"));
	
	AGDGameModeBase* GDGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(GDGameMode))
	{
		return;
	}
	
	UCharacterClassInfo* ClassInfo = GDGameMode->CharacterClassInfo;
	checkf(ClassInfo, TEXT("ClassInfo is null"));
	checkf(ClassInfo->SecondaryAttributes, TEXT("ClassInfo->SecondaryAttributes is null"));
	checkf(ClassInfo->VitalAttributes, TEXT("ClassInfo->VitalAttributes is null"));
	
	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	
	FGameplayEffectSpecHandle PrimaryEffectSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, EffectContextHandle);
	FGameplayEffectSpecHandle SecondaryEffectSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, EffectContextHandle);
	FGameplayEffectSpecHandle VitalEffectSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, EffectContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data);
	
}

void UGDAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	checkf(WorldContextObject, TEXT("WorldContextObject is null"));
	checkf(ASC, TEXT("ASC is null"));
	
	AGDGameModeBase* GDGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(GDGameMode))
	{
		return;
	}

	UCharacterClassInfo* ClassInfo = GDGameMode->CharacterClassInfo;
	checkf(ClassInfo, TEXT("ClassInfo is null"));

	for (TSubclassOf<UGameplayAbility> CommonAbility : ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}
