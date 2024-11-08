// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"


AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
}
