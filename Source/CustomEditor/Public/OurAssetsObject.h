// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OurAssetsObject.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CUSTOMEDITOR_API UOurAssetsObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Our Assets")
	FName AssetName;
};
