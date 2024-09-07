// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "GDAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

UCLASS()
class GDPROJECT_API AGDAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// 行为树组件，在CharacterEnemy中设置
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComp;
	
public:
	AGDAIController();
	
};
