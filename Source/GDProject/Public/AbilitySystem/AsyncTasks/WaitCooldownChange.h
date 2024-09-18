// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"


struct FGameplayTag;
struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangedSignature, float, TimeRemaining);

/**
 * 等待指定能力冷却变化的异步任务。
 * 当指定能力的冷却变化时，会回调
 * ExposedAsyncProxy 表示这个Task是异步的Task，需要在结束时调用 EndTask
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class GDPROJECT_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangedSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangedSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& CooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	void OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
};
