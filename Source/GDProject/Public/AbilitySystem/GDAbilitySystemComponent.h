// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GDAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectTagApplied, const FGameplayTagContainer&)

/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	FOnEffectTagApplied OnEffectTagApplied;
	
public:
	UFUNCTION()
	void OnAbilityActorInfoSet();
	
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupAbilities);

	// 处理输入事件
	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);
	void OnAbilityInputHeld(FGameplayTag InputTag);
};
