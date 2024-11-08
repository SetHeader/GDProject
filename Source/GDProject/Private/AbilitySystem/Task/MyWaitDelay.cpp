// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Task/MyWaitDelay.h"

#include "AbilitySystem/Task/MyWaitDelayResource.h"

UMyWaitDelay::UMyWaitDelay(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	Time = 0.f;
	TimeStarted = 0.f;

	bCaresAboutPriority = true;

	AddRequiredResource(UMyWaitDelayResource::StaticClass());
	AddClaimedResource(UMyWaitDelayResource::StaticClass());
}

void UMyWaitDelay::Activate()
{
	Super::Activate();
	
	UWorld* World = GetWorld();
	if (Time <= 0.f)
	{
		World->GetTimerManager().SetTimerForNextTick(this, &UMyWaitDelay::OnTimeFinish);
	}
	else
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &UMyWaitDelay::OnTimeFinish, Time, false);
	}
}

UMyWaitDelay* UMyWaitDelay::TaskWaitDelay(TScriptInterface<IGameplayTaskOwnerInterface> TaskOwner, float Time,
	const uint8 Priority)
{
	UMyWaitDelay* MyWaitDelay = NewTaskUninitialized<UMyWaitDelay>();
	if (MyWaitDelay && TaskOwner.GetInterface() != nullptr)
	{
		MyWaitDelay->InitTask(*TaskOwner, Priority);
		MyWaitDelay->Time = Time;
	}
	return MyWaitDelay;
}

void UMyWaitDelay::Pause()
{
	Super::Pause();

	UWorld* World = GetWorld();
	World->GetTimerManager().PauseTimer(TimerHandle);
}

void UMyWaitDelay::Resume()
{
	Super::Resume();

	UWorld* World = GetWorld();
	World->GetTimerManager().UnPauseTimer(TimerHandle);
}

void UMyWaitDelay::OnTimeFinish()
{
	OnFinishDelegate.Broadcast();
	EndTask();
}
