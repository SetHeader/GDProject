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
	switch (InLevel)
	{
		case 1:
			return TEXT("能够发射 1 个火球，增加少量伤害");
		case 2:
			return TEXT("能够发射 2 个火球，增加少量伤害");
		case 3:
			return TEXT("能够发射 3 个火球，增加少量伤害");
		case 4:
			return TEXT("能够发射 4 个火球，增加少量伤害");
		case 5:
			return TEXT("能够发射 5 个火球，增加少量伤害");
		case 6:
			return TEXT("能够发射 6 个火球，增加少量伤害");
		case 7:
			return TEXT("能够发射 7 个火球，增加少量伤害");
		default:
			return TEXT("");
	}
}

FString UFireBoltAbility::GetNextLevelDescription(int32 InLevel)
{
	return GetDescription(InLevel + 1);
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
