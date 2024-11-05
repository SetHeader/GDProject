// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GDProjectileSpellAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GDGameplayTags.h"
#include "Actor/GDProjectile.h"
#include "Characters/GDCharacterBase.h"

void UGDProjectileSpellAbility::SpawnProjectile(const FVector Destination, const FGameplayTag& SocketTag, const float Pitch, const bool bOverridePitch, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		return;
	}

	check(ProjectileClass)
	
	FTransform SpawnTransform;

	ICombatInterface* Owner = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!Owner)
	{
		return;
	}
	// 接口中的BlueprintCallable方法，需要用Execute_的方式调用
	FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	SpawnTransform.SetLocation(SocketLocation);
	FRotator Rotation = (Destination - SocketLocation).Rotation();
	Rotation.Pitch = 0;
	if (bOverridePitch)
	{
		Rotation.Pitch = Pitch;
	} else
	{
		Rotation.Pitch += Pitch;
	}
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	AGDProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGDProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
	Projectile->FinishSpawning(SpawnTransform);
}
