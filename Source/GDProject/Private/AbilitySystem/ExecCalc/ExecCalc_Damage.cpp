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
	// 定义属性标签和属性的映射
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FGDGameplayTags& Tags = FGDGameplayTags::Get();
		
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_Armor, GDDamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_BlockChange, GDDamageStatics().BlockChangeDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_ArmorPenetration, GDDamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitChange, GDDamageStatics().CriticalHitChangeDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitResistance, GDDamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitDamage, GDDamageStatics().CriticalHitDamageDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, GDDamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, GDDamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lightning, GDDamageStatics().LightingResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, GDDamageStatics().PhysicalResistanceDef);
	
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetAvatar = TargetASC ? SourceASC->GetAvatarActor() : nullptr;

	int SourceCombatLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourceCombatLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int TargetCombatLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetCombatLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

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
	
	// 应用Debuff
	DetermineDebuff(ExecutionParams, EffectSpec, EvaluationParameters, TagsToCaptureDefs);

	// 获取伤害
	float Damage = 0.f;  
	// 计算伤害与伤害抵抗
	for (const auto& DamageType : FGDGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag& DamageTypeTag = DamageType.Key;
		const FGameplayTag& ResistanceTypeTag = DamageType.Value;

		checkf(TagsToCaptureDefs.Contains(ResistanceTypeTag), TEXT("ExecCalc_Damage\t TagToDef doesn't contain Tag: [%s]"), *ResistanceTypeTag.ToString());

		const FGameplayEffectAttributeCaptureDefinition ResistanceTypeDef = TagsToCaptureDefs[ResistanceTypeTag];
		
		const float DamageTypeValue = EffectSpec.GetSetByCallerMagnitude(DamageTypeTag, false);
		float TargetResistance = CaptureAttributeValue(ExecutionParams, EvaluationParameters, ResistanceTypeDef);
		
		TargetResistance = FMath::Clamp(TargetResistance, 0.f, 100.f);
		
		Damage += DamageTypeValue * (100.f - TargetResistance) / 100.f;
	}
	
	// 获取伤害计算中的各种系数
	const float ArmorPenetrationCoeff = GetCoeficientInCurve(SourceAvatar, FName("ArmorPenetration"), SourceCombatLevel);
	const float EffectiveArmorCoeff = GetCoeficientInCurve(SourceAvatar, FName("EffectiveArmor"), TargetCombatLevel);
	// 暴击伤害抵抗系数，范围 0-1，该系统越小，造成的暴击伤害越低
	const float CriticalHitResistanceCoeff = GetCoeficientInCurve(SourceAvatar, FName("CriticalHitResistance"), TargetCombatLevel);
	
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

float UExecCalc_Damage::GetCoeficientInCurve(const UObject* WorldContextObject, FName RealCurveName, float Time) const
{
	const UCurveTable* CurveTable = UGDAbilitySystemLibrary::GetDamageCoefficientsCurveTable(WorldContextObject);
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

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& Spec, FAggregatorEvaluateParameters EvaluationParameters,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const
{
	const FGDGameplayTags& GameplayTags = FGDGameplayTags::Get();

	for (TTuple<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag& DamageType = Pair.Key;
		const FGameplayTag& DebuffType = Pair.Value;
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, 0.f);
		if (TypeDamage > 0.f)
		{
			// 判断是否是成功的debuff
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);

			// 目标Debuff抗性
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluationParameters, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			// 最终的Debuff生效概率
			const float EffectiveDebuffChance = SourceDebuffChance * ( 100 - TargetDebuffResistance ) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle ContextHandle = Spec.GetEffectContext();
				
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, 0.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);
				
				UGDAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);
				UGDAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UGDAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UGDAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UGDAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}
