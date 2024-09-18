// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "PlayerInterface.generated.h"


struct FGameplayTag;

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};


class GDPROJECT_API IPlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP();
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 XP);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(const int32 InLevel) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(const int32 InLevel) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints();
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InLevel);

	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InPoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InPoints);
};
