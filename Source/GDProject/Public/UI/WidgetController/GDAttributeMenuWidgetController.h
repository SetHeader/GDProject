// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDAttributeWidgetController.h"
#include "GDAttributeMenuWidgetController.generated.h"



UCLASS(BlueprintType, Blueprintable)
class GDPROJECT_API UGDAttributeMenuWidgetController : public UGDAttributeWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnStatChangedSignature OnAttributePointsChangedDelegate;

	void BroadcastInitialValues() const override;
	void BindCallbacksToDependencies() const override;

	UFUNCTION(BlueprintCallable, Category = "GDAttributeMenuWidgetController")
	void UpgradeAttributePoint(FGameplayTag Tag);
};
