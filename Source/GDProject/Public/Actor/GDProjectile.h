// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GDProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class GDPROJECT_API AGDProjectile : public AActor
{
	GENERATED_BODY()

public:
	AGDProjectile();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USphereComponent> CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;
    
	UPROPERTY(EditAnywhere, Category="GDProjectile")
	TObjectPtr<USoundBase> ImpactSound;
	
	UPROPERTY(EditAnywhere, Category="GDProjectile")
    TObjectPtr<USoundBase> ImpactSOund;
};
