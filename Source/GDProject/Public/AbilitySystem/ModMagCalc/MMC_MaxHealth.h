// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

struct FGameplayEffectAttributeCaptureDefinition;
struct FGameplayEffectSpec;

/**
 * 
 */
UCLASS()
class GDPROJECT_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
protected:
	FGameplayEffectAttributeCaptureDefinition VigorDef;
	
public:
	UMMC_MaxHealth();

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

};
