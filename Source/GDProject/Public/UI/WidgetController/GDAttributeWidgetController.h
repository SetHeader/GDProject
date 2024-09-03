// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDWidgetController.h"
#include "AbilitySystem/Data/GDAttributeInfo.h"
#include "GDAttributeWidgetController.generated.h"

struct FOnAttributeChangeData;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeInfoChangedSignature, const FAttributeInfo, AttributeInfo);

UCLASS()
class GDPROJECT_API UGDAttributeWidgetController : public UGDWidgetController
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
};
