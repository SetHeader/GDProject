// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValidDataSignature, const FGameplayAbilityTargetDataHandle&, DataDataHandle);

UCLASS()
class GDPROJECT_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="Tasks")
	FOnValidDataSignature OnValidDataDelegate;
	
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", BlueprintInternalUseOnly,
		meta=(DisplayName = "TargetDataUnderMouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;
	
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
