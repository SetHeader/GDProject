// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTask.h"
#include "MyWaitDelay.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTaskDelaySignature);

UCLASS()
class GDPROJECT_API UMyWaitDelay : public UGameplayTask
{
	GENERATED_BODY()

public:
	UMyWaitDelay(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FTaskDelaySignature OnFinishDelegate;

	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, Category="GameplayTasks", meta=(AdvancedDisplay="TaskOwner, Priority", DefaultToSelf="TaskOwner", BlueprintInternalUseOnly="TRUE"))
	static UMyWaitDelay* TaskWaitDelay(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, float Time, const uint8 Priority = 192);

	virtual void Pause() override;
	virtual void Resume() override;

private:
	void OnTimeFinish();

	float Time;
	float TimeStarted;

	FTimerHandle TimerHandle;
};
