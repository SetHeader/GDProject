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
	/**
	 * 获取指定等级的魔力消耗
	 * @param InLevel InLevel默认为-1表示当前能力等级
	 * @return 魔力消耗
	 */
	int32 GetManaCost(int32 InLevel = -1.f) const;
	
	// 获取指定等级的冷却，InLevel默认为-1表示当前能力等级
	float GetCooldown(float InLevel = -1.f) const;
};
