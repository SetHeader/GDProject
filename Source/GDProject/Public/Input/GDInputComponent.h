// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GDInputComponent.generated.h"

struct FGameplayTag;
class UGDInputConfig;
/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, class PressedFuncType, class ReleasedFuncType, class HeldFuncType>
	void SetAbilityInputConfig(const UGDInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};
