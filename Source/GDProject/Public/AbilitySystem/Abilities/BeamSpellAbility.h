// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GDDamageGameplayAbility.h"
#include "BeamSpellAbility.generated.h"

class UNiagaraSystem;

UCLASS()
class GDPROJECT_API UBeamSpellAbility : public UGDDamageGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	FVector MouseHitLocation;
	
	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category="Beam")
	TObjectPtr<ACharacter> OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category="Beam")
	int32 MaxNumShockTargets = 5;
public:

	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();
	
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);

	virtual FString GetDescription(int32 InLevel) override;
	virtual FString GetNextLevelDescription(int32 InLevel) override;
};
