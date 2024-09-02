// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/GDAttributeInfo.h"
#include "UI/WidgetController/GDWidgetController.h"
#include "GDAttributeMenuWidgetController.generated.h"

class UGDAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfoChangedSignature, const FAttributeInfo, AttributeInfo);

UCLASS(BlueprintType, Blueprintable)
class GDPROJECT_API UGDAttributeMenuWidgetController : public UGDWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UGDAttributeInfo> AttributesInfo;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeInfoChangedSignature OnAttributeChanged;

	void BroadcastInitialValues() const override;

	void BindCallbacksToDependencies() const override;

	void BroadcastAttributeChanged(const FOnAttributeChangeData& ChangedData, FGameplayTag AttributeTag) const ;

	UFUNCTION(BlueprintCallable, Category = "GDAttributeMenuWidgetController")
	void AddValue(FGameplayTag Tag);
};
