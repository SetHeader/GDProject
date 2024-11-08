// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "WaitInputPressed.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInputChanged);

/**
 * 等待
 */
UCLASS()
class GDPROJECT_API UWaitInputPressed : public UAbilityTask
{
	GENERATED_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnInputChanged OnInputChangedDelegate;

	FGameplayTag InputTag;
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", BlueprintInternalUseOnly,
		meta=(DisplayName = "WaitInputChanged", HidePin="OwningAbility", DefaultToSelf="OwningAbility"))
	static UWaitInputPressed* CreateWaitInputPressed(UGameplayAbility* OwningAbility, FGameplayTag WaitInputTag);

	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;
	
	UFUNCTION()
	void OnInputTagPressed(const FGameplayTag& InInputTag);
};
