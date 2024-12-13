// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickupableActor.h"

#include "InventoryStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"


APickupableActor::APickupableActor() {
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(GetRootComponent());
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APickupableActor::OnBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APickupableActor::OnEndOverlap);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(CollisionComponent);
}

void APickupableActor::BeginPlay() {
	Super::BeginPlay();
	
}

void APickupableActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult) {

	if (HasAuthority()) {
		UInventoryManagerComponent* InventoryComponent = UInventoryStatics::GetInventoryManagerComponent(OtherActor);
		if (InventoryComponent) {
			InventoryComponent->AddItem(UInventoryStatics::GenerateItemInstance(ItemDefinition), StackCount);
			// TODO 应该在客户端播放声音
			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
			Destroy();
		}
	}
}

void APickupableActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex) {
	
}


