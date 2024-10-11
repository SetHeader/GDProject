// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDGameplayAbility.h"
#include "GDDamageGameplayAbility.generated.h"

/**
 * 带伤害的能力，支持多种不同类型的伤害
 */
UCLASS()
class GDPROJECT_API UGDDamageGameplayAbility : public UGDGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 不同类型的伤害值
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void CauseDamage(AActor* TargetActor);
};
