// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/FireBoltAbility.h"

#include "Actor/GDProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

void UFireBoltAbility::SpawnProjectile(const FVector Destination, const FGameplayTag& SocketTag, const float Pitch,
                                       const bool bOverridePitch, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	int32 Count = FMath::Clamp(GetAbilityLevel(), 1, MaxNumProjectiles);
	
	SpawnMultipleProjectile(Count, Destination, SocketTag, HomingTarget);
}

FString UFireBoltAbility::GetDescription(int32 InLevel)
{
	int32 ProjectileCount = FMath::Min(InLevel, MaxNumProjectiles);
	
	FString ExtraDesc = FString::Printf(
		TEXT("<Default>发射</><Special>%d</><Default>个火球，每个火球造成伤害: </><Damage>%d</>"),
		ProjectileCount,
		static_cast<int>(GetDamageAtLevel(InLevel)));
	
	return GetDescTemplate(InLevel, TEXT("火球术"), ExtraDesc);
}

FString UFireBoltAbility::GetNextLevelDescription(int32 InLevel)
{
	int32 NextLevel = InLevel + 1;
	
	int32 CurrProjectileCount = FMath::Min(InLevel, MaxNumProjectiles);
	int32 NextProjectileCount = FMath::Min(NextLevel, MaxNumProjectiles);
	
	FString ExtraDesc = FString::Printf(TEXT("<Default>火球数量：</><Special>%d</> -> <Special>%d</>\n"),
			CurrProjectileCount,
			NextProjectileCount);
	
	return GetNextLevelDescTemplate(InLevel, ExtraDesc);
}

FString UFireBoltAbility::GetLockedDescription()
{
	return TEXT("能够发射火球，升级后能发射多个火球");
}

void UFireBoltAbility::SpawnMultipleProjectile(int32 Count, const FVector Destination,
	const FGameplayTag& SocketTag, AActor* HomingTarget)
{
	check(ProjectileClass)
	check(Count > 0)
	
	ICombatInterface* Owner = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!Owner)
	{
		return;
	}
	
	// 接口中的BlueprintCallable方法，需要用Execute_的方式调用
	FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);

	for (int i = 0; i < Count; i++)
	{
		FTransform SpawnTransform = FTransform();

		SpawnTransform.SetLocation(SocketLocation);

		// 向前上方发射，在下落的时候跟踪最近的敌人
	
		FRotator Rotation = (Destination - SocketLocation).Rotation();
	
		Rotation.Pitch = 60;
		if (Count > 1)
		{
			Rotation.Yaw += -ProjectileSpread / 2 + i * ProjectileSpread / (Count - 1);
		}

		SpawnTransform.SetRotation(Rotation.Quaternion());
	
		AGDProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGDProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->MovementComponent->bIsHomingProjectile = bHomingProjectile;
		Projectile->MovementComponent->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);;
		
		// 如果有指定敌人，就跟踪敌人，否则就跟踪指定位置
		if (HomingTarget)
		{
			Projectile->MovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(Destination);
			Projectile->MovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}

		Projectile->MovementComponent->ProjectileGravityScale = 3;

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
		Projectile->FinishSpawning(SpawnTransform);
	}
}
