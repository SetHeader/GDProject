// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "OurAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMEDITOR_API UOurAssetFactory : public UFactory
{
	GENERATED_BODY()
public:
	UOurAssetFactory();

	virtual bool CanCreateNew() const override { return true; }
	virtual bool ShouldShowInNewMenu() const override { return true; }
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
