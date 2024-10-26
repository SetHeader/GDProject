// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GDGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// 当前玩家的起点位置
	UPROPERTY()
	FName PlayerStartTag = FName();

	// 当前加载的存档槽名字
	UPROPERTY()
	FString LoadSlotName = FString();
	
	// 当前加载的存档槽下标
	UPROPERTY()
	int32 LoadSlotIndex = 0;

	
};
