// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BeamSpellAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/NetworkMetricsDefs.h"

void UBeamSpellAbility::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UBeamSpellAbility::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UBeamSpellAbility::SpawnElectricBeam(UNiagaraSystem* NiagaraSystem)
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetAvatarActorFromActorInfo(), NiagaraSystem, FVector());
}

void UBeamSpellAbility::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation("TipSocket");
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResult,
				true);

			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
	}
	// 监听Actor死亡
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamSpellAbility::PrimaryTargetDied))
		{
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UBeamSpellAbility::PrimaryTargetDied);
		}
	}
}

void UBeamSpellAbility::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	if (!MouseHitActor->Implements<UEnemyInterface>())
	{
		return;
	}
	TArray<AActor*> OverlayActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MouseHitActor);
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
		
	UGDAbilitySystemLibrary::GetLivePlayersWithInRadius(
		GetAvatarActorFromActorInfo(),
		OverlayActors,
		ActorsToIgnore,
		850.f,
		MouseHitActor->GetActorLocation());

	// int32 NumAdditionTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	int32 NumAdditionTargets = 5;

	UGDAbilitySystemLibrary::GetClosestTargets(
		NumAdditionTargets,
		OverlayActors,
		OutAdditionalTargets,
		MouseHitActor->GetActorLocation());

	// 监听Actor死亡
	for (AActor* OutAdditionalTarget : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(OutAdditionalTarget))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &UBeamSpellAbility::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UBeamSpellAbility::AdditionalTargetDied);
			}
		}
	}
}
