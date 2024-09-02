// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GDInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FGDInputAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	const UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * 输入配置，用来把输入动作 和 标签关联起来。
 * 因为能力GA 也是和标签关联起来的，所以输入动作触发时 能通过标签 触发能力
 */
UCLASS(BlueprintType)
class GDPROJECT_API UGDInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GDInputConfig")
	TArray<FGDInputAction> AbilityInputActions;

	const UInputAction* FindAbilityInputAction(const FGameplayTag InputTag, bool bLogNotFound = true) const;
};
