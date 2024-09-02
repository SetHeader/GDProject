// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/GDProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
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
	AudioComponent = UGameplayStatics::SpawnSoundAttached(PersistentSound, GetRootComponent());
}

void AGDProjectile::Destroyed()
{
	// 没碰到东西的话，在销毁时也要生成效果
	if (!bHit && !HasAuthority())
	{
		UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
		if (AudioComponent) AudioComponent->Stop();
	}
	Super::Destroyed();
}

void AGDProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 不管是在客户端还是服务端，碰撞到东西就生成碰撞效果
	UGameplayStatics::SpawnSoundAtLocation(this, ImpactSound, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (AudioComponent) AudioComponent->Stop();
	
	// 在服务销毁自身，销毁动作会复制到客户端上
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			if (DamageEffectHandle.IsValid())
			{
				ASC->ApplyGameplayEffectSpecToSelf(*DamageEffectHandle.Data.Get());
			}
		}
		
		Destroy();
	} else
	{
		bHit = true;
	}
}
