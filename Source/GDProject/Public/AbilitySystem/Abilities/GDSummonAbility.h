// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDGameplayAbility.h"
#include "GDSummonAbility.generated.h"

/**
 * 召唤技能，能召唤随从
 */
UCLASS()
class GDPROJECT_API UGDSummonAbility : public UGDGameplayAbility
{
	GENERATED_BODY()

public:
	// 生成几个随从
	UPROPERTY(EditAnywhere, Category="Summoning")
	int32 NumMinions = 5;
	// 随从类
	UPROPERTY(EditAnywhere, Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;
	// 限制最小召唤距离
	UPROPERTY(EditAnywhere, Category="Summoning")
	float MinSpawnDistance = 50.f;
	// 限制最大召唤距离
	UPROPERTY(EditAnywhere, Category="Summoning")
	float MaxSpawnDistance = 250.f;
	// 限制召唤区域的角度，如90度就是 向前向量 左转45度 和 右转45度 之间的区域
	UPROPERTY(EditAnywhere, Category="Summoning")
	float SpawnSpread = 90.f;
	

	// 获取每个随从的召唤位置
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintCallable)
	TSubclassOf<APawn> GetRandomMinionClass();
};
