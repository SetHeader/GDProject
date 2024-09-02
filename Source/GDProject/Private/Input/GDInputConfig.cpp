// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/GDInputConfig.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"

const UInputAction* UGDInputConfig::FindAbilityInputAction(const FGameplayTag InputTag, bool bLogNotFound) const
{
	for (const FGDInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("%s\t InputAction Not Found! Tag [%s]"), *GetNameSafe(this), *InputTag.ToString());
	}
	
	return nullptr;
}
