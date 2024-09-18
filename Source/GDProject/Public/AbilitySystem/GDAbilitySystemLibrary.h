// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/GDAttributeMenuWidgetController.h"
#include "UI/WidgetController/GDOverlayWidgetController.h"
#include "GDAbilitySystemLibrary.generated.h"

struct FGameplayEffectContextHandle;
/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="GDAbilitySystemLibrary|WidgetController")
	static UGDOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="GDAbilitySystemLibrary|WidgetController")
	static UGDAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="GDAbilitySystemLibrary|CharacterClassDefault")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="GDAbilitySystemLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category="GDAbilitySystemLibrary|CharacterClassDefault")
	static const UCurveTable* GetDamageCoefficientsCurveTable(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="GDAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="GDAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="GDAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bValue);

	UFUNCTION(BlueprintCallable, Category="GDAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bValue);

	UFUNCTION(BlueprintCallable, Category="GDAbilitySystemLibrary|GameplayEffects")
	static void GetLivePlayersWithInRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GDAbilitySystemLibrary|GameplayEffects")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GDAbilitySystemLibrary|Character")
	static ECharacterClass GetCharacterClass(AActor* Character);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GDAbilitySystemLibrary|Character")
	static int32 GetCharacterLevel(const AActor* Character);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="GDAbilitySystemLibrary|Character")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};
