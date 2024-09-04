// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GDInputConfig.h"
#include "GDInputComponent.generated.h"

/**
 * 自定义的增强输入组件，用来把输入键、输入标记、输入动作 关联起来
 */
UCLASS()
class GDPROJECT_API UGDInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template<class UserClass, class PressedFuncType, class ReleasedFuncType, class HeldFuncType>
	void SetAbilityInputConfig(const UGDInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, class PressedFuncType, class ReleasedFuncType, class HeldFuncType>
void UGDInputComponent::SetAbilityInputConfig(const UGDInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig)
	check(Object)
	
	for (const FGDInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (PressedFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
		}
		if (ReleasedFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
		}
		if (HeldFunc)
		{
			BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
		}
	}
}