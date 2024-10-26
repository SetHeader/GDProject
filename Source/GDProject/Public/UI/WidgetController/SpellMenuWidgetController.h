// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/GDAttributeInfo.h"
#include "SpellMenuWidgetController.generated.h"

class UAbilityInfo;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoChangedSignature, FGDAbilityInfo, AbilityInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilitySelected, FGameplayTag, SelectedAbility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpellPointsChangedSignature, const int32, SpellPoints);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GDPROJECT_API USpellMenuWidgetController : public UGDWidgetController
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintAssignable)
	FAbilityInfoChangedSignature OnAbilityInfoChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAbilitySelected OnAbilitySelected;

	UPROPERTY(BlueprintAssignable)
	FOnSpellPointsChangedSignature OnSpellPointsChangedDelegate;

	// 能力信息，从GameMode中获取的
	UAbilityInfo* AbilityInfo;

	// 当前选中的能力
	FGameplayTag SelectedAbilityTag;
	
public:
	virtual void BroadcastInitialValues() const override;
	virtual void BindCallbacksToDependencies() const override;

	// 选中某个能力
	UFUNCTION(BlueprintCallable)
	void SelectAbility(FGameplayTag AbilityTag);

	void SetAbilityInfo(UAbilityInfo* InAbilityInfo) { AbilityInfo = InAbilityInfo; }

	UFUNCTION(BlueprintCallable)
	FGDAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag) const;
	
	// 升级能力
	UFUNCTION(BlueprintCallable)
	void UpgradeAbility(FGameplayTag AbilityTag) const;

	// 装备能力
	UFUNCTION(BlueprintCallable)
	void EquipAbility(FGameplayTag AbilityTag, FGameplayTag InputTag) const;
	
};
 