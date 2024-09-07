// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "GDEffectActor.generated.h"

class UPrimitiveComponent;
class USphereComponent;
class UStaticMeshComponent;
struct FHitResult;
class UGameplayEffect;
class UAbilitySystemComponent;

UENUM()
enum class EInstantEffectApplicationPolicy {
	DoNoApply,
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap
};

UENUM()
enum class EDurationEffectApplicationPolicy {
	DoNoApply,
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap
};

UENUM()
enum class EInfiniteEffectApplicationPolicy {
	DoNoApply,
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap
};

UENUM()
enum class EInfiniteEffectRemovalPolicy {
	DoNoRemove,
	RemoveOnEndOverlap
};



UCLASS()
class GDPROJECT_API AGDEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// 立即效果类
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDEffectActor")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	// 立即效果应用策略
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDEffectActor")
	EInstantEffectApplicationPolicy InstantEffectApplicationPolicy = EInstantEffectApplicationPolicy::DoNoApply;
	// 持续效果类
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDEffectActor")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	// 持续效果应用策略
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDEffectActor")
	EDurationEffectApplicationPolicy DurationEffectApplicationPolicy = EDurationEffectApplicationPolicy::DoNoApply;
	// 永久效果类
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDEffectActor")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	// 永久效果应用策略
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDEffectActor")
	EInfiniteEffectApplicationPolicy InfiniteEffectApplicationPolicy = EInfiniteEffectApplicationPolicy::DoNoApply;
	// 永久效果移除策略
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDEffectActor")
	EInfiniteEffectRemovalPolicy InfiniteEffectRemovalPolicy = EInfiniteEffectRemovalPolicy::DoNoRemove;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDEffectActor")
	float ActorLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GDEffectActor")
	bool bDestroyOnEffectApplication = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="GDEffectActor")
	bool bApplyEffectsToEnemies = false;
private:
	TMap<FActiveGameplayEffectHandle, TPair<UAbilitySystemComponent*, TSubclassOf<UGameplayEffect>>> CachedEffectMap;

public:	
	UFUNCTION(BlueprintCallable, Category = "GDEffectActor")
	void ApplyEffectToActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Category = "GDEffectActor")
	void RemoveEffectFromActor(AActor* TargetActor, TSubclassOf<UGameplayEffect> EffectClass);

	UFUNCTION(BlueprintCallable, Category = "GDEffectActor")
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category = "GDEffectActor")
	void OnEndOverlap(AActor* TargetActor);
};
