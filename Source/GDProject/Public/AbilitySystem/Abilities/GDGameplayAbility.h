// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GDGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// 默认的输入标签，会添加到DynamicAbilityTags中。不要在编码中修改该属性。
	UPROPERTY(EditDefaultsOnly, Category="GDGameplayAbility")
	FGameplayTag SetupInputTag;
	
	// 获取描述
	virtual FString GetDescription(int32 InLevel);
	// 获取下一级的描述
	virtual FString GetNextLevelDescription(int32 InLevel);
	// 获取未解锁时的描述
	virtual FString GetLockedDescription();

protected:
	float GetManaCost(float InLevel = 1.f) const;
	float GetCooldown(float InLevel = 1.f) const;
};
