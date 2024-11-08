// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyTestActor.generated.h"

UCLASS()
class GDPROJECT_API AMyTestActor : public AActor
{
	GENERATED_BODY()

public:
	AMyTestActor* MyActor1;

	TObjectPtr<AMyTestActor> MyActor2;

	UPROPERTY()
	AMyTestActor* MyActor3;
	
	UPROPERTY()
	TObjectPtr<AMyTestActor> MyActor4;

	AMyTestActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TestGenerate();

	UFUNCTION(BlueprintCallable)
	void TestDestroy();

	UFUNCTION(BlueprintCallable)
	void TestFun();
};