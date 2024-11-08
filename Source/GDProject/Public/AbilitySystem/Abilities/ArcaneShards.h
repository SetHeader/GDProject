// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

UCLASS()
class GDPROJECT_API UArcaneShards : public UGDDamageGameplayAbility
{
	GENERATED_BODY()

public:
	// 获取当前等级的说明
	virtual FString GetDescription(int32 InLevel) override;
	/**
	 * 获取下一级的说明
	 * @param InLevel 当前级，获取到的时InLevel + 1 级的说明
	 */
	virtual FString GetNextLevelDescription(int32 InLevel) override;
	virtual FString GetLockedDescription() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxShardsCount = 8;
	
};
