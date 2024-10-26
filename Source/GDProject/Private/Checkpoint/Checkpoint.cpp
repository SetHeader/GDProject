// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint/Checkpoint.h"

#include "Components/SphereComponent.h"
#include "Game/GDGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"


ACheckpoint::ACheckpoint(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetupAttachment(RootComponent);
	
	CheckpointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	CheckpointMesh->SetCollisionResponseToAllChannels(ECR_Block);
	CheckpointMesh->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	CheckpointMesh->SetupAttachment(Sphere);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnSphereOverlap);
}

void ACheckpoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("Player")))
	{
		bReached = true;
		if (AGDGameModeBase* GameModeBase = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			GameModeBase->SaveWorldState(GetWorld());
		}
		
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects();
	}
}

void ACheckpoint::HandleGlowEffects()
{
	if (!DynamicMaterialInst)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DynamicMaterialInst = UMaterialInstanceDynamic::Create(CheckpointMesh->GetMaterial(0), this);
		CheckpointMesh->SetMaterial(0, DynamicMaterialInst);
	}
	
	Glow(DynamicMaterialInst);
}

void ACheckpoint::FinishHandleGlowEffects()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void ACheckpoint::LoadActor_Implementation()
{
	// 触发一次特效
	if (bReached)
	{
		HandleGlowEffects();
	}
}

bool ACheckpoint::ShouldLoadTransform_Implementation()
{
	return false;
}
