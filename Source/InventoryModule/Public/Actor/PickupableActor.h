// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupableActor.generated.h"

class UInventoryItemDefinition;
class UInventoryItemInstance;
class USphereComponent;

/**
 * 可以拾取的物品，拾取后自动添加到物品栏中
 */
UCLASS()
class INVENTORYMODULE_API APickupableActor : public AActor {
	GENERATED_BODY()

public:
	APickupableActor();
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickupableActor")
	TObjectPtr<USphereComponent> CollisionComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PickupableActor")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickupableActor")
	TSubclassOf<UInventoryItemDefinition> ItemDefinition;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PickupableActor")
	int32 StackCount = 1;
	
	// 拾取时的音效
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PickupableActor")
	TObjectPtr<USoundBase> PickupSound;

	
};
