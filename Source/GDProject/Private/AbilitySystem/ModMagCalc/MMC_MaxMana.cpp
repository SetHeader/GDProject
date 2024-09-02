// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"
#include "GameplayEffectAttributeCaptureDefinition.h"
#include "AbilitySystem/AttributeSets/GDAttributeSetBase.h"
#include "GameplayEffect.h"
#include "Characters/Heroes/GDCharacterHero.h"



UMMC_MaxMana::UMMC_MaxMana()
{
	// 定义要捕获的属性
	IntelligenceDef.AttributeToCapture = UGDAttributeSetBase::GetVigorAttribute();
	// 从GE的目标捕获属性
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	IntelligenceDef.bSnapshot = false;

	// 要捕获的与计算相关的属性
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.0f);


	if (AGDCharacterHero* Hero = Cast<AGDCharacterHero>(Spec.GetContext().GetSourceObject())) {
		return  50 + 2 * Intelligence + Hero->GetPlayerLevel() * 4;
	}

	return 0;
}
