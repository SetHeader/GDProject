// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "GDProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class GDPROJECT_API AGDProjectile : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;

	// 持续性的音效，发射物生成后就自带
	UPROPERTY(EditAnywhere, Category="GDProjectile")
	TObjectPtr<USoundBase> PersistentSound;

	// 碰撞时音效
	UPROPERTY(EditAnywhere, Category="GDProjectile")
	TObjectPtr<USoundBase> ImpactSound;

	// 碰撞时粒子效果
	UPROPERTY(EditAnywhere, Category="GDProjectile")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	FGameplayEffectSpecHandle DamageEffectHandle;

	TObjectPtr<UAudioComponent> AudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float LifeSpan = 15.f;
private:
	
	bool bHit = false;

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	AGDProjectile();
	
	virtual void Destroyed() override;

protected:
	virtual void BeginPlay() override;
};
