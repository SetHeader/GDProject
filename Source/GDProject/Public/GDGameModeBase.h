// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GDGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
/**
 * 游戏模式
 */
UCLASS()
class GDPROJECT_API AGDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	// 配置敌人和主角的默认属性、默认能力
	UPROPERTY(EditDefaultsOnly, Category="GDGameModeBase")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	// 配置主角的所有技能定义
	UPROPERTY(EditDefaultsOnly, Category="GDGameModeBase")
	TObjectPtr<UAbilityInfo> AbilityInfo;
};
