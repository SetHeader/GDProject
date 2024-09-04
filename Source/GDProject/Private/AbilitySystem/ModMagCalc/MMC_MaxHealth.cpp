// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "GameplayEffectAttributeCaptureDefinition.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "GameplayEffect.h"
#include "Characters/Heroes/GDCharacterHero.h"



UMMC_MaxHealth::UMMC_MaxHealth()
{
	// 定义要捕获的属性
	VigorDef.AttributeToCapture = UGDAttributeSet::GetVigorAttribute();
	// 从GE的目标捕获属性
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	VigorDef.bSnapshot = false;

	// 要捕获的与计算相关的属性
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.0f);


	if (AGDCharacterBase* Character = Cast<AGDCharacterBase>(Spec.GetContext().GetSourceObject())) {
		return  80 + 2.5 * Vigor + Character->GetPlayerLevel() * 5;
	}

	return 0;
}
