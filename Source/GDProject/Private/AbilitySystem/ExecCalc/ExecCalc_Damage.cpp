// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "Interaction/CombatInterface.h"



/**
 * 原始类型，不会暴露给蓝图，不用生成ue反射代码，就不需要用'F'开头 
 */
struct GDDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChange)
	
	GDDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, BlockChange, Target, false)
	}
};

/**
 * 获取实例的静态方法，永远只有一个实例
 */
static const GDDamageStatics& DamageStatic()
{
	static GDDamageStatics Instance;
	return Instance;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	// 定义要捕获哪些属性
	RelevantAttributesToCapture.Add(DamageStatic().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatic().BlockChangeDef);
	RelevantAttributesToCapture.Add(DamageStatic().ArmorPenetrationDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? SourceASC->GetAvatarActor() : nullptr;
	
	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	
	const int SourceCombatLevel = SourceCombatInterface ? SourceCombatInterface->GetPlayerLevel() : 1;
	const int TargetCombatLevel = TargetCombatInterface ? TargetCombatInterface->GetPlayerLevel() : 1;
	
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluationParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// 获取伤害
	float Damage = EffectSpec.GetSetByCallerMagnitude(FGDGameplayTags::Get().Damage);

	// 获取伤害计算中的各种系数
	float ArmorPenetrationCoeff = GetCoeficientInCurve(FName("ArmorPenetration"), SourceCombatLevel);
	float EffectiveArmorCoeff = GetCoeficientInCurve(FName("EffectiveArmor"), TargetCombatLevel);
	
	// 获取 Target的BlockChange，决定是否要格挡成功
	// 格挡成功要将伤害减半
	float TargetBlockChange = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().BlockChangeDef);
	float RandomValue = FMath::RandRange(0, 100);

	if (RandomValue <= TargetBlockChange)
	{
		Damage = Damage / 2;
	}

	// 敌人护甲
	float TargetArmor = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().ArmorDef);

	// 护甲穿透要按比例忽略敌方的护甲
	float SourceArmorPenetration = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().ArmorPenetrationDef);

	// 应用护甲穿透后，敌人的有效护甲。即每四点护甲穿透，能穿透 1% 的护甲
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoeff) / 100.f;

	// 每 3点 有效护甲减免 1% 的伤害
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoeff) / 100.f;
	
	// 应用伤害
	if (Damage > 0.f)
	{
		FGameplayModifierEvaluatedData DamageEvaluatedData(UGDAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
		OutExecutionOutput.AddOutputModifier(DamageEvaluatedData);
	}
}

float UExecCalc_Damage::CaptureAttributeValue(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FAggregatorEvaluateParameters& EvaluateParameters,
	const FGameplayEffectAttributeCaptureDefinition& AttrDef) const
{
	float Value = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttrDef, EvaluateParameters, Value);
	return FMath::Max(0.f, Value);
}

float UExecCalc_Damage::GetCoeficientInCurve(FName RealCurveName, float Time) const
{
	const UCurveTable* CurveTable = UGDAbilitySystemLibrary::GetDamageCoefficientsCurveTable(GetWorld());
	if (CurveTable)
	{
		const FRealCurve* RealCurve = CurveTable->FindCurve(RealCurveName, FString());
		if (RealCurve)
		{
			return RealCurve->Eval(Time);
		}
	}
	
	UE_LOG(LogTemp, Error, TEXT("UExecCalc_Damage:\t Can't found CurveTable or RealCurve. In %s"), *RealCurveName.ToString());
	return 0.f;
}
