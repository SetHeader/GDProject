// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"

#include "SaveInterface.generated.h"


UINTERFACE(MinimalAPI, BlueprintType)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 存档接口，要存档的Actor需要实现此接口。且只有实现此接口的Actor才能存档数据。
 */
class GDPROJECT_API ISaveInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldLoadTransform();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadActor();
};
