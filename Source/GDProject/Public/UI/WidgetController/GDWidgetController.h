// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GDWidgetController.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;


/**
 * 
 */
UCLASS(BlueprintType)
class GDPROJECT_API UGDWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY(BlueprintReadOnly, Category = "GDWidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "GDWidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "GDWidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "GDWidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UFUNCTION(BlueprintCallable, Category = "GDWidgetController")
	void SetParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
	{
		PlayerController = PC;
		PlayerState = PS;
		AbilitySystemComponent = ASC;
		AttributeSet = AS;
		BindCallbacksToDependencies();
	}

	virtual void BroadcastInitialValues() const {}

	virtual void BindCallbacksToDependencies() const {}
};
