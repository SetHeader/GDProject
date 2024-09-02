// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/GDInputComponent.h"

#include "GameplayTagContainer.h"
#include "Input/GDInputConfig.h"

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