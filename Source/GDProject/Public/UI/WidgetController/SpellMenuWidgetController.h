﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "SpellMenuWidgetController.generated.h"

class UAbilityInfo;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoChangedSignature, FGDAbilityInfo, AbilityInfo);

/**
 * 
 */
UCLASS()
class GDPROJECT_API USpellMenuWidgetController : public UGDWidgetController
{
	GENERATED_BODY()

private:
	FAbilityInfoChangedSignature OnAbilityInfoChangedDelegate;
	
	UAbilityInfo* AbilityInfo;
public:
	virtual void BroadcastInitialValues() const override;
	virtual void BindCallbacksToDependencies() const override;

	UFUNCTION(BlueprintCallable)
	FGDAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag) const;
	
	// 升级能力
	UFUNCTION(BlueprintCallable)
	void UpgradeAbility(FGameplayTag AbilityTag) const;

	// 装备能力
	UFUNCTION(BlueprintCallable)
	void EquipAbility(FGameplayTag AbilityTag, FGameplayTag InputTag) const;
};
 