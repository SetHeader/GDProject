// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GDDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/GDAbilityTypes.h"

void UGDDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	check(TargetActor);
	
	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	
	const float DamageValue = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageType, DamageValue);
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC)
	{
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("GDDamageGameplayAbility\t TargetActor Is Not a ASC. [%s]"), *TargetActor->GetName());
	}
}

FDamageEffectParams UGDDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor,
	const FVector& InRadialDamageOrigin, bool bOverrideKnockbackDirection, const FVector& KnockbackDirectionOverride,
	bool bOverrideDeathImpulse, const FVector& DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;

	FRotator RotatorFromAvatarToTarget = FRotator::ZeroRotator;
	if (TargetActor)
	{
		RotatorFromAvatarToTarget = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
	}
	
	if (FMath::RandRange(1, 100) <= KnockbackChance)
	{
		Params.KnockbackChance = KnockbackChance;
		Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
		if (bOverrideKnockbackDirection)
		{
			Params.KnockbackForce = KnockbackDirectionOverride.GetSafeNormal() * KnockbackForceMagnitude;
		}
		else
		{
			Params.DeathImpulse = RotatorFromAvatarToTarget.Vector() * KnockbackForceMagnitude;
		}
	}
	
	if (bOverridePitch)
	{
		RotatorFromAvatarToTarget.Pitch = PitchOverride;
	}
	
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	if (bOverrideDeathImpulse)
	{
		Params.DeathImpulse = DeathImpulseDirectionOverride.GetSafeNormal() * DeathImpulseMagnitude;
	}
	else
	{
		Params.DeathImpulse = RotatorFromAvatarToTarget.Vector() * DeathImpulseMagnitude;
	}
	
	Params.bIsRadialDamage = bIsRadialDamage;
	Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
	Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	Params.RadialDamageOrigin = InRadialDamageOrigin;
	return Params;
}


float UGDDamageGameplayAbility::GetDamageAtLevel(int InLevel) const
{
	return Damage.GetValueAtLevel(InLevel);
}

FString UGDDamageGameplayAbility::GetDescTemplate(int32 CurrLevel, FString Title, FString ExtraDesc)
{
	return FString::Printf(TEXT(
			"<Title>%s</>\n\n"
			
			"<Small>等级: </><Level>%d</>\n"
			"<Small>魔力消耗: </><ManaCost>%d</>\n"
			"<Small>冷却: </><Cooldown>%.1f s</>\n\n"
			
			"%s\n"
		), *Title, CurrLevel, GetManaCost(CurrLevel), GetCooldown(CurrLevel), *ExtraDesc);
}

FString UGDDamageGameplayAbility::GetNextLevelDescTemplate(int32 CurrLevel, FString ExtraDescription)
{
	int32 NextLevel = CurrLevel + 1;
	
	int32 CurrDaamge = static_cast<int>(Damage.GetValueAtLevel(CurrLevel));
	int32 NextDaamge = static_cast<int>(Damage.GetValueAtLevel(NextLevel));

	int32 CurrManaCost = GetManaCost(CurrLevel);
	int32 NextManaCost = GetManaCost(NextLevel);

	float CurrCooldown = GetCooldown(CurrLevel);
	float NextCooldown = GetCooldown(NextLevel);
	
	FString Desc = FString::Printf(TEXT(
			"<Default>下一级: </><Level>%d</>\n\n"
		));
	
	FString ManaDesc = CurrManaCost == NextManaCost ?
		FString::Printf(TEXT("<Default>魔力消耗: </><ManaCost>%d</>"), CurrManaCost) :
		FString::Printf(TEXT("<Default>魔力消耗: </><ManaCost>%d</> -> <ManaCost>%d</>"), CurrManaCost, NextManaCost);

	FString CooldownDesc = CurrCooldown == NextCooldown ?
		FString::Printf(TEXT("<Default>冷却: </><Cooldown>%.1f s</>"), CurrCooldown) :
		FString::Printf(TEXT("<Default>冷却: </><Cooldown>%.1f s</> -> <Cooldown>%.1f s</>"), CurrCooldown, NextCooldown);
	
	FString DamageDesc = CurrDaamge == NextDaamge ?
			FString::Printf(TEXT("<Default>伤害: </><Damage>%d</>"), CurrDaamge) :
			FString::Printf(TEXT("<Default>伤害: </><Damage>%d</> -> <Damage>%d</>"), CurrDaamge, NextDaamge);
	
	return FString::Printf(TEXT(
			"<Default>下一级: </><Level>%d</>\n\n"
			
			"%s\n"
			"%s\n"
			"%s\n"
			"%s"
		),
		NextLevel,
		*ManaDesc,
		*CooldownDesc,
		*DamageDesc,
		*ExtraDescription);
}
