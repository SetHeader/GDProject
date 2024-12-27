// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GDEntryGameModeBase.h"

#include "Kismet/GameplayStatics.h"

void AGDEntryGameModeBase::BeginPlay() {
	Super::BeginPlay();
	
	Maps.Add(DefaultMapName, DefaultMap);
}

void AGDEntryGameModeBase::TravelToMap(const FString& MapName) {
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Maps.FindChecked(MapName));
}
