// Fill out your copyright notice in the Description page of Project Settings.


#include "OurAssetFactory.h"

#include "OurAssetsObject.h"

UOurAssetFactory::UOurAssetFactory()
{
	SupportedClass = UOurAssetsObject::StaticClass();
}

UObject* UOurAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
                                            UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UOurAssetsObject>(InParent, InClass, InName, Flags, Context);
}
