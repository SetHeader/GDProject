// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GDEffectActor.h"
#include "GameplayEffect.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void AGDEffectActor::ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!ASC) {
		return;
	}

	check(EffectClass);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(EffectClass, ActorLevel, EffectContextHandle);
	FActiveGameplayEffectHandle ActiveEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	CachedEffectMap.Add(ActiveEffectHandle, { ASC, EffectClass });
}

void AGDEffectActor::RemoveEffectFromActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!ASC) {
		return;
	}

	check(EffectClass);

	TArray<FActiveGameplayEffectHandle> NeedRemovalHandles;
	for (TPair<FActiveGameplayEffectHandle, TPair<UAbilitySystemComponent*, TSubclassOf<UGameplayEffect>>>& Pair : CachedEffectMap) {
		if (Pair.Value.Key == ASC && EffectClass == Pair.Value.Value) {
			NeedRemovalHandles.Add(Pair.Key);
		}
	}

	for (FActiveGameplayEffectHandle Handle : NeedRemovalHandles) {
		ASC->RemoveActiveGameplayEffect(Handle, 1);
	}
}

void AGDEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!ASC) {
		return;
	}

	bool bIsEnemy = TargetActor->ActorHasTag(FName("Enemy"));
	if (bIsEnemy && !bApplyEffectsToEnemies)
	{
		return;
	}

	if (InstantEffectApplicationPolicy == EInstantEffectApplicationPolicy::ApplyOnBeginOverlap) {
		ApplyEffectToActor(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EDurationEffectApplicationPolicy::ApplyOnBeginOverlap) {
		ApplyEffectToActor(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EInfiniteEffectApplicationPolicy::ApplyOnBeginOverlap) {
		ApplyEffectToActor(TargetActor, InfiniteGameplayEffectClass);
	}

	if (bDestroyOnEffectApplication)
	{
		Destroy();
	}
}

void AGDEffectActor::OnEndOverlap(AActor* TargetActor)
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!ASC) {
		return;
	}

	if (InstantEffectApplicationPolicy == EInstantEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToActor(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EDurationEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToActor(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EInfiniteEffectApplicationPolicy::ApplyOnEndOverlap) {
		ApplyEffectToActor(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EInfiniteEffectRemovalPolicy::RemoveOnEndOverlap) {
		RemoveEffectFromActor(TargetActor, InfiniteGameplayEffectClass);
	}
}
