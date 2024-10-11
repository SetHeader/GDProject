// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GDWidgetController.generated.h"

class UGDAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStatChangedSignature, int32, NewValue);

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

	UFUNCTION(BlueprintCallable, Category = "GDWidgetController")
	virtual void BroadcastInitialValues() const {}

	virtual void BindCallbacksToDependencies() const {}

	UGDAbilitySystemComponent* GetGDASC() const;
};
