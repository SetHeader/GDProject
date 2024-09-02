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
	UPROPERTY(EditDefaultsOnly, Category="GDGameplayAbilit")
	FGameplayTag SetupTag;
};
