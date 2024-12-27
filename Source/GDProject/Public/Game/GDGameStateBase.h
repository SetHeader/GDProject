// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GDProject/GDProject.h"
#include "GDGameStateBase.generated.h"

class UAbilityInfo;
class UCharacterClassInfo;

UCLASS(BlueprintType, Blueprintable)
class GDPROJECT_API AGDGameStateBase : public AGameStateBase {
	GENERATED_BODY()
	
public:
	// 配置敌人和主角的默认属性、默认能力
	UPROPERTY(EditDefaultsOnly, Category="AGDGameStateBase")
	TObjectPtr<const UCharacterClassInfo> CharacterClassInfo;

	// 配置主角的所有技能定义，不要修改配置的内容
	UPROPERTY(EditDefaultsOnly, Category="AGDGameStateBase")
	TObjectPtr<const UAbilityInfo> AbilityInfo;
};
