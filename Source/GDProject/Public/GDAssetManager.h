// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GDAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
private:
	static UGDAssetManager* INSTANCE;
	
	
public:
	static UGDAssetManager& Get();

	virtual void StartInitialLoading() override;

};
