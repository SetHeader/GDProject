// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

struct FGameplayEffectAttributeCaptureDefinition;
struct FGameplayEffectSpec;

/**
 * 
 */
UCLASS()
class GDPROJECT_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
protected:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
	
public:
	UMMC_MaxMana();

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
