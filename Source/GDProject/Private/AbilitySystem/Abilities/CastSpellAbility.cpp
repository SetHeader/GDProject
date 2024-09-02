// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CastSpellAbility.h"

#include "Actor/GDProjectile.h"
#include "Characters/GDCharacterBase.h"

void UCastSpellAbility::SpawnProjectile(const FRotator Rotation)
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
	
	SpawnTransform.SetLocation(Owner->GetCombatSocketLocation());
	SpawnTransform.SetRotation(Rotation.Quaternion());
	
	AGDProjectile* Projectile = GetWorld()->SpawnActorDeferred<AGDProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->FinishSpawning(SpawnTransform);
}
