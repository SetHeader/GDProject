﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDGameplayAbility.h"
#include "CastSpellAbility.generated.h"

class AGDProjectile;

UCLASS()
class GDPROJECT_API UCastSpellAbility : public UGDGameplayAbility
{
	GENERATED_BODY()

public:
	/**
	 * 发射物类，需要在蓝图中配置
	 */
	UPROPERTY(EditDefaultsOnly, Category = "CastSpellAbility")
	TSubclassOf<AGDProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "CastSpellAbility")
	TSubclassOf<UGameplayEffect> EffectClass;
protected:
	/**
	 * 朝某个方向生成发射物，只在服务端生成发射物
	 * @param Rotation 发射方向
	 */
	UFUNCTION(BlueprintCallable, Category = "CastSpellAbility")
	virtual void SpawnProjectile(const FRotator Rotation);

};