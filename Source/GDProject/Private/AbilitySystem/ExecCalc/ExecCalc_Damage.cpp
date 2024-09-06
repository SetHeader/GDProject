// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/GDAbilityTypes.h"
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
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChange)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightingResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToDef;

	GDDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, Armor, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, ArmorPenetration, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, BlockChange, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, CriticalHitChange, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, CriticalHitDamage, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, CriticalHitResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, FireResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, LightingResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, ArcaneResistance, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGDAttributeSet, PhysicalResistance, Target, false)
		
		// 不能在构造函数中初始化映射，因为构造函数中的FGDGameplayTags也没初始化，Tag全是None。
		if (FGDGameplayTags::Get().IsInitialized())
		{
			Initialize();
		}
		else
		{
			InitializeHandle = FGDGameplayTags::Get().OnInitedDelegate.AddRaw(this, &GDDamageStatics::Initialize);
		}
	}

private:
	bool bInitialized = false;
	FDelegateHandle InitializeHandle;

public:
	void Initialize()
	{
		FGDGameplayTags::Get().OnInitedDelegate.Remove(InitializeHandle);
		const FGDGameplayTags& Tags = FGDGameplayTags::Get();
		TagToDef.Add(Tags.Attribute_Secondary_Armor, ArmorDef);
		TagToDef.Add(Tags.Attribute_Secondary_ArmorPenetration, ArmorPenetrationDef);
		TagToDef.Add(Tags.Attribute_Secondary_BlockChange, BlockChangeDef);
		TagToDef.Add(Tags.Attribute_Secondary_CriticalHitChange, CriticalHitChangeDef);
		TagToDef.Add(Tags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		TagToDef.Add(Tags.Attribute_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		TagToDef.Add(Tags.Attribute_Resistance_Fire, FireResistanceDef);
		TagToDef.Add(Tags.Attribute_Resistance_Lighting, LightingResistanceDef);
		TagToDef.Add(Tags.Attribute_Resistance_Arcane, ArcaneResistanceDef);
		TagToDef.Add(Tags.Attribute_Resistance_Physical, PhysicalResistanceDef);
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
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitChangeDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatic().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().LightingResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatic().PhysicalResistanceDef);
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
	
	// 捕获属性值，并限制值要大于等于0
	const float TargetBlockChange = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().BlockChangeDef);
	const float TargetArmor = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().ArmorDef);
	const float SourceArmorPenetration = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().ArmorPenetrationDef);
	const float SourceCriticalHitChange = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().CriticalHitChangeDef);
	const float SourceCriticalHitDamage = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().CriticalHitDamageDef);
	const float TargetCriticalHitResistance = CaptureAttributeValue(ExecutionParams, EvaluationParameters, DamageStatic().CriticalHitResistanceDef);
	
	// 获取伤害
	float Damage = 0.f;
	// 计算伤害与伤害抵抗
	for (const auto& DamageType : FGDGameplayTags::Get().DamageTypesToResistance)
	{
		const FGameplayTag& DamageTypeTag = DamageType.Key;
		const FGameplayTag& ResistanceTypeTag = DamageType.Value;

		checkf(DamageStatic().TagToDef.Contains(ResistanceTypeTag), TEXT("ExecCalc_Damage\t TagToDef doesn't contain Tag: [%s]"), *ResistanceTypeTag.ToString());

		const FGameplayEffectAttributeCaptureDefinition ResistanceTypeDef = DamageStatic().TagToDef[ResistanceTypeTag];
		
		const float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag);
		float TargetResistance = CaptureAttributeValue(ExecutionParams, EvaluationParameters, ResistanceTypeDef);
		
		TargetResistance = FMath::Clamp(TargetResistance, 0.f, 100.f);
		
		Damage += DamageTypeValue * (100.f - TargetResistance) / 100.f;
	}
	
	// 获取伤害计算中的各种系数
	const float ArmorPenetrationCoeff = GetCoeficientInCurve(FName("ArmorPenetration"), SourceCombatLevel);
	const float EffectiveArmorCoeff = GetCoeficientInCurve(FName("EffectiveArmor"), TargetCombatLevel);
	// 暴击伤害抵抗系数，范围 0-1，该系统越小，造成的暴击伤害越低
	const float CriticalHitResistanceCoeff = GetCoeficientInCurve(FName("CriticalHitResistance"), TargetCombatLevel);
	
	// 获取 Target的BlockChange，决定是否要格挡成功
	// 格挡成功要将伤害减半
	const bool bBlock = FMath::RandRange(0, 100) <= TargetBlockChange;
	
	if (bBlock)
	{
		Damage = Damage / 2;
	}

	// 应用护甲穿透后，敌人的有效护甲。即每四点护甲穿透，能穿透 1% 的护甲
	float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoeff) / 100.f;
	EffectiveArmor = FMath::Max(0.f, EffectiveArmor);
	
	// 每 3点 有效护甲减免 1% 的伤害
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoeff) / 100.f;
	
	// 有效暴击率。每6.666个暴击抵抗点数 能降低 1% 的暴击率
	const float EffectiveCriticalHitChange = SourceCriticalHitChange - TargetCriticalHitResistance * CriticalHitResistanceCoeff;
	// 是否暴击
	const bool bCriticalHit = (EffectiveCriticalHitChange) <= FMath::RandRange(0, 100);
	
	if (bCriticalHit)
	{
		// 暴击就产生2倍伤害，再加上额外伤害
		Damage = Damage * 2.f + SourceCriticalHitDamage;
	}

	// 向上取整
	Damage = FMath::CeilToInt(Damage);
	Damage = FMath::Max(0.f, Damage);

	// 设置一些信息到效果上下文
	FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
	UGDAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlock);
	UGDAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	
	// 应用伤害
	FGameplayModifierEvaluatedData DamageEvaluatedData(UGDAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(DamageEvaluatedData);
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
