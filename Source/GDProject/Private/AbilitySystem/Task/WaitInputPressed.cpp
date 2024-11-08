// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Task/WaitInputPressed.h"

#include "AbilitySystem/GDAbilitySystemComponent.h"


UWaitInputPressed* UWaitInputPressed::CreateWaitInputPressed(UGameplayAbility* OwningAbility, FGameplayTag WaitInputTag)
{
	UWaitInputPressed* Task = NewAbilityTask<UWaitInputPressed>(OwningAbility);
	Task->InputTag = WaitInputTag;
	
	return Task;
}

void UWaitInputPressed::Activate()
{
	Super::Activate();

	if (UGDAbilitySystemComponent* ASC = Cast<UGDAbilitySystemComponent>(Ability->GetAbilitySystemComponentFromActorInfo()))
	{
		ASC->OnInputTagPressedDelegate.AddUObject(this, &UWaitInputPressed::OnInputTagPressed);
	}
}

void UWaitInputPressed::OnDestroy(bool bInOwnerFinished)
{
	if (UGDAbilitySystemComponent* ASC = Cast<UGDAbilitySystemComponent>(Ability->GetAbilitySystemComponentFromActorInfo()))
	{
		ASC->OnInputTagPressedDelegate.RemoveAll(this);
	}

	Super::OnDestroy(bInOwnerFinished);
}

void UWaitInputPressed::OnInputTagPressed(const FGameplayTag& InInputTag)
{
	if (InInputTag == InputTag)
	{
		OnInputChangedDelegate.Broadcast();
		EndTask();
	}
}
