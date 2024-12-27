// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GDGameModeBase.h"

#include "GameFramework/PlayerStart.h"
#include "GDProject/GDProject.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/SaveGame/SaveGameSubsystem.h"

void AGDGameModeBase::TravelToMap(const FString& MapName)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Maps.FindChecked(MapName));
}

AActor* AGDGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	USaveGameSubsystem* SaveGameSubsystem = UGameInstance::GetSubsystem<USaveGameSubsystem>(GetGameInstance());
	if (!SaveGameSubsystem) {
		GDLog::WarningStatic(TEXT("AGDGameModeBase"), TEXT("SaveGameSubsystem Not Found"));
		return nullptr;
	}
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		APlayerStart* PlayerStart = CastChecked<APlayerStart>(Actor);
		if (PlayerStart->PlayerStartTag == SaveGameSubsystem->PlayerStartTag)
		{
			return PlayerStart;
		}
	}
	
	return Actors.IsEmpty() ? nullptr : Actors[0];
}

void AGDGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	Maps.Add(DefaultMapName, DefaultMap);
	
	if (GetNetMode() == NM_DedicatedServer) {
		// 全局只有一个世界存档
		// 加载存档中的地图
		if (auto SaveGameSubsystem = UGameInstance::GetSubsystem<USaveGameSubsystem>(GetGameInstance()))
			SaveGameSubsystem->GetAndLoadWorldSaveGame(GetWorld());

		// 定时存档
		GetWorld()->GetTimerManager().SetTimer(SaveWorldTimerHandle, this, &AGDGameModeBase::SaveWorld, 60, true);
	}
}

void AGDGameModeBase::BeginDestroy() {
	Super::BeginDestroy();

	if (GetNetMode() == NM_DedicatedServer) {
		GetWorld()->GetTimerManager().ClearTimer(SaveWorldTimerHandle);
	}
}

void AGDGameModeBase::SaveWorld() {
	if (auto SaveGameSubsystem = UGameInstance::GetSubsystem<USaveGameSubsystem>(GetGameInstance())) {
		SaveGameSubsystem->SetWorldSaveGame(GetWorld());
	} else {
		GDLog::WarningStatic(TEXT("AGDGameModeBase"), TEXT("SaveGameSubsystem Not Found"));
	}
}
