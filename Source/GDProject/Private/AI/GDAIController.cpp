// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/GDAIController.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"


AGDAIController::AGDAIController()
{
	PrimaryActorTick.bCanEverTick = false;

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
}
