// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GDProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GDProject/GDProject.h"
#include "Kismet/GameplayStatics.h"

AGDProjectile::AGDProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionComponent");
	CollisionComponent->InitSphereRadius(10);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECC_Projectile);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SetRootComponent(CollisionComponent);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 1000;
	MovementComponent->MaxSpeed = 1000;
	MovementComponent->ProjectileGravityScale = 0.f;
}

void AGDProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSpan);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGDProjectile::OnBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AGDProjectile::OnEndOverlap);
	AudioComponent = UGameplayStatics::SpawnSoundAttached(PersistentSound, GetRootComponent());
}

void AGDProjectile::PlayHitEffect(FVector InLocation)
{
	if (ImpactSound) 
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, InLocation, FRotator::ZeroRotator);
	if (ImpactEffect)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, InLocation);
}

void AGDProjectile::PlayHitEffect() {
	PlayHitEffect(GetActorLocation());
}

void AGDProjectile::CauseDamage(AActor* TargetActor) const {
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
		return;
	FDamageEffectParams ParamsInst = DamageEffectParams;
	
	const bool bKnockback = FMath::RandRange(1, 100) < ParamsInst.KnockbackChance;
	if (bKnockback)
	{
		FRotator Rotation = GetActorRotation();
		Rotation.Pitch = 45.f;
				
		const FVector KnockbackDirection = Rotation.Vector();
		const FVector KnockbackForce = KnockbackDirection * ParamsInst.KnockbackForceMagnitude;
		ParamsInst.KnockbackForce = KnockbackForce;
	}
		
	ParamsInst.TargetAbilitySystemComponent = TargetASC;
	UGDAbilitySystemLibrary::ApplyDamageEffect(ParamsInst);
}

void AGDProjectile::CauseDamageAndPlayHitEffect(AActor* TargetActor) {
	if (HasAuthority()) {
		CauseDamage(TargetActor);
	} else {
		PlayHitEffect(TargetActor->GetActorLocation());
	}
}

void AGDProjectile::StartDamageTimer(AActor* TargetActor) {
	if (OverlappingActorsToTimerHandle.Contains(TargetActor)) {
		return;
	}
	
	OverlappingActorsToTimerHandle.Add(TargetActor, FTimerHandle());
	
	FTimerHandle& TimerHandle = OverlappingActorsToTimerHandle.FindChecked(TargetActor);
	
	CauseDamageAndPlayHitEffect(TargetActor);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&, TargetActor = TObjectPtr<AActor>(TargetActor)] {
		if (TargetActor) {
			CauseDamageAndPlayHitEffect(TargetActor);
		}
	}, DamageFrequency, true);
}

void AGDProjectile::StopDamageTimer(AActor* TargetActor) {
	if (!OverlappingActorsToTimerHandle.Contains(TargetActor)) {
		return;
	}
	
	FTimerHandle& TimerHandle = OverlappingActorsToTimerHandle.FindChecked(TargetActor);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void AGDProjectile::Destroyed()
{
	Super::Destroyed();
	// 没碰到东西的话，在销毁时也要生成效果
	if (!bHit && !HasAuthority()) PlayHitEffect();
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
	}
	TArray<TObjectPtr<AActor>> Keys;
	OverlappingActorsToTimerHandle.GetKeys(Keys);
	for (TObjectPtr<AActor> Actor : Keys) {
		FTimerHandle TimerHandle = OverlappingActorsToTimerHandle.FindAndRemoveChecked(Actor);
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void AGDProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 不会对自己造成伤害
	if (OtherActor == DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor())
		return;

	if (bDestroyedOnHit) {
		if (bHit) 
			return;
		
		bHit = true;
		
		PlayHitEffect();
		
		// 在服务销毁自身，销毁动作会复制到客户端上
		if (HasAuthority())
		{
			CauseDamage(OtherActor);
			Destroy();
		}
	} else {
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		if (TargetASC)
			StartDamageTimer(OtherActor);
	}
}

void AGDProjectile::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex) {
	if (OtherActor == DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor())
		return;
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (TargetASC)
		StopDamageTimer(OtherActor);
}
