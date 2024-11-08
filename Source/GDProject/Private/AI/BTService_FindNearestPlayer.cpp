// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = AIOwner->GetPawn();
	
	if (!IsValid(OwningPawn))
	{
		return;
	}
	
	// 如果以后有宠物，那么就需要区分目标是 Enemy 还是 Player。敌人的目标是Player，宠物的目标是Enemy
	const FName TargetTag = OwningPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(OwningPawn, TargetTag, Actors);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;
	
	for (AActor* Actor : Actors)
	{
		if (!IsValid(Actor) || ICombatInterface::Execute_IsDead(Actor))
		{
			continue;
		}
		float Distance = OwningPawn->GetDistanceTo(Actor);
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}

	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
}
