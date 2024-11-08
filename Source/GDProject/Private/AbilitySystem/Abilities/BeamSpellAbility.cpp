// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BeamSpellAbility.h"

#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/EnemyInterface.h"
#include "Kismet/GameplayStatics.h"

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

void UBeamSpellAbility::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			
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
				EDrawDebugTrace::None,
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

	int32 NumAdditionTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);

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

FString UBeamSpellAbility::GetDescription(int32 InLevel)
{
	int32 ShockCount = FMath::Min(InLevel, MaxNumShockTargets);
	
	FString ExtraDesc = FString::Printf(
		TEXT("<Default>生成一条闪电链,击中目标同时也会链接附近的至多%d个敌人，每秒造成5次</><Damage>%d</><Default>伤害，并有概率晕眩目标。</>"),
		ShockCount,
		static_cast<int>(GetDamageAtLevel(InLevel))
		);
	
	return GetDescTemplate(InLevel, TEXT("闪电链"), ExtraDesc);
}

FString UBeamSpellAbility::GetNextLevelDescription(int32 InLevel)
{
	int32 NextLevel = InLevel + 1;
	
	int32 NextShockCount = FMath::Min(NextLevel, MaxNumShockTargets);
	
	FString ExtraDesc = FString::Printf(TEXT("<Default>链接附近的至多</><Special>%d</><Default>个敌人</>\n"),
			NextShockCount);
	
	return GetNextLevelDescTemplate(InLevel, ExtraDesc);
}
