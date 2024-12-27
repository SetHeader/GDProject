// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/PickupableActor.h"

#include "InventoryStatics.h"
#include "Components/SphereComponent.h"
#include "GDProject/GDProject.h"
#include "Inventory/InventoryItemDefinition.h"
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
	
	if (!ItemDefinition) {
		GDLOG_W(TEXT("PickupableActor"), TEXT("%s ItemDefinition未定义"), *GetName());
		return;
	}
	
	UInventoryManagerComponent* InventoryComponent = UInventoryStatics::GetInventoryManagerComponent(OtherActor);
	if (InventoryComponent) {
		int32 AddedCount = InventoryComponent->AddItem(UInventoryStatics::GenerateItemInstance(ItemDefinition), StackCount);
		StackCount -= AddedCount;
		if (StackCount == 0) {

			if (HasLocalNetOwner()) {
				GDLOG(TEXT("PickupableActor"), TEXT("HasLocalNetOwner"));
			}
			if (HasNetOwner()) {
				GDLOG(TEXT("PickupableActor"), TEXT("HasNetOwner"));
			}
			if (!HasAuthority()) {
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
			}
			
			Destroy();
		}
	}
}

void APickupableActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex) {
	
}


