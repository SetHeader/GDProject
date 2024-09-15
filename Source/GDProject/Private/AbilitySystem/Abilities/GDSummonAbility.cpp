// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GDSummonAbility.h"

TArray<FVector> UGDSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	const FVector RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);

	// 画出生成区域的左边界与右边界
	DrawDebugLine(GetWorld(), Location + MinSpawnDistance * LeftOfSpread, Location + MaxSpawnDistance * LeftOfSpread, FColor::Green);
	DrawDebugLine(GetWorld(), Location + MinSpawnDistance * RightOfSpread, Location + MaxSpawnDistance * RightOfSpread, FColor::Green);
	
	TArray<FVector> SpawnLocations;
	
	const float DeltaSpread = SpawnSpread / NumMinions;

	for (int i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		
		DrawDebugLine(GetWorld(), Location + MinSpawnDistance * Direction, Location + MaxSpawnDistance * Direction, FColor::Green);
		
		FVector SpawnLocation = Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		
		// 对于凹凸不平的地方需要找到合适的召唤位置
		FHitResult HitResult;
		FVector Offset(0, 0, 400);
		GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation + Offset, SpawnLocation - Offset, ECC_Visibility);
		if (HitResult.bBlockingHit)
		{
			SpawnLocation = HitResult.ImpactPoint;
		}
		
		DrawDebugSphere(GetWorld(), SpawnLocation, 16.f, 8.f, FColor::Green);
		
		SpawnLocations.Add(SpawnLocation);
	}

	return SpawnLocations;
}

TSubclassOf<APawn> UGDSummonAbility::GetRandomMinionClass()
{
	int32 Idx = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses.IsEmpty() ? nullptr : MinionClasses[Idx];
}
