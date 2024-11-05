// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDProjectileSpellAbility.h"
#include "FireBoltAbility.generated.h"

/**
 * 施放火球的能力
 */
UCLASS()
class GDPROJECT_API UFireBoltAbility : public UGDProjectileSpellAbility
{
	GENERATED_BODY()

public:
	/**
	 * 是否启用归航功能，默认启用
	 */
	UPROPERTY(EditDefaultsOnly)
	bool bHomingProjectile = true;

	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMin = 1500;

	UPROPERTY(EditDefaultsOnly)
	float HomingAccelerationMax = 2500;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpread = 60.f;

	UPROPERTY(EditDefaultsOnly)
	int32 MaxNumProjectiles = 5;
	
	virtual void SpawnProjectile(const FVector Destination, const FGameplayTag& SocketTag, const float Pitch, const bool bOverridePitch, AActor* HomingTarget = nullptr) override;

	virtual FString GetDescription(int32 InLevel) override;

	virtual FString GetNextLevelDescription(int32 InLevel) override;

	virtual FString GetLockedDescription() override;

private:
	/**
	 * 生成多个抛射物
	 * @param Count 抛射物数量 
	 * @param YawRange 航向角范围
	 * @param Destination 目标
	 * @param SocketTag 哪个部位发射的
	 */
	void SpawnMultipleProjectile(int32 Count, const FVector Destination, const FGameplayTag& SocketTag, AActor* HomingTarget = nullptr);
};
