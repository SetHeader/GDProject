// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/GDAttributeMenuWidgetController.h"
#include "UI/WidgetController/GDOverlayWidgetController.h"
#include "GDAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="GDAbilitySystemLibrary|WidgetController")
	static UGDOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="GDAbilitySystemLibrary|WidgetController")
	static UGDAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
};
