// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CastSpellAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GDGameplayTags.h"
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

	UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());

	// 设置伤害到SpecHandle里面。能够在 GE蓝图编辑器中的 SetByCaller 中通过Damage标签访问
	FGameplayTag DamageTag = FGDGameplayTags::Get().Damage;
	const float DamageValue = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageTag, DamageValue);

	Projectile->DamageEffectHandle = SpecHandle;
	
	Projectile->FinishSpawning(SpawnTransform);
}
