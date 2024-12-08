// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * 自定义的伤害执行器，相比于MMC，ExecCalc更加灵活，支持同时修改多个属性。
 */
UCLASS()
class GDPROJECT_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	float CaptureAttributeValue(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FAggregatorEvaluateParameters& EvaluateParameters, const FGameplayEffectAttributeCaptureDefinition& AttrDef) const;

	float GetCoeficientInCurve(const UObject* WorldContextObject, FName RealCurveName, float Time) const;

	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
						 const FGameplayEffectSpec& Spec,
						 FAggregatorEvaluateParameters EvaluationParameters,
						 const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;

private:
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	FDelegateHandle InitHandle;
};
