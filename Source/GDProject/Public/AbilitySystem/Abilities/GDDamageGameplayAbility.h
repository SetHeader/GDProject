// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDGameplayAbility.h"
#include "GDDamageGameplayAbility.generated.h"

struct FDamageEffectParams;

/**
 * 带伤害的能力，支持多种不同类型的伤害
 */
UCLASS()
class GDPROJECT_API UGDDamageGameplayAbility : public UGDGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 不同类型的伤害值
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	// Debuff发生的概率
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;
	// Debuff造成的伤害
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;
	// Debuff触发的频率
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;
	// Debuff的持续时长
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;
	// 死亡击飞的冲量大小
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;
	// 击退的冲量大小
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;
	// 击退的概率
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

public:
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure, meta=(AutoCreateRefTerm="InRadialDamageOrigin,KnockbackDirectionOverride, DeathImpulseDirectionOverride"))
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr, const FVector& InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false, const FVector& KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false, const FVector& DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false, float PitchOverride = 0.f) const;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetDamageAtLevel(int InLevel) const;

protected:
	
	virtual FString GetDescTemplate(int32 CurrLevel, FString Title, FString ExtraDesc);

	virtual FString GetNextLevelDescTemplate(int32 CurrLevel, FString ExtraDescription);
};
