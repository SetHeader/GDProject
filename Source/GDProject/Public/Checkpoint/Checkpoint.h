// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

UCLASS()
class GDPROJECT_API ACheckpoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

private:
	UMaterialInstanceDynamic* DynamicMaterialInst;

public:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;
	
	ACheckpoint(const FObjectInitializer& ObjectInitializer);
protected:
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	// 发光。即靠近存档点时发光
	void HandleGlowEffects();
	
	// 蓝图实现，具体发光效果的实现逻辑
	UFUNCTION(BlueprintImplementableEvent)
	void Glow(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintCallable)
	void FinishHandleGlowEffects();
public:
	/** Start SaveInterface */
	virtual void LoadActor_Implementation() override;
	virtual bool ShouldLoadTransform_Implementation() override;
	/** End SaveInterface */
};
