// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SaveGame/SaveGameSubsystem.h"

#include "EngineUtils.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Subsystem/SaveGame/PlayerSaveGame.h"
#include "Subsystem/SaveGame/WorldSaveGame.h"

class UGDGameInstance;
class FActorIterator;
class USaveGame;

USaveGameSubsystem::USaveGameSubsystem(): Logger(SimpleLogger{TEXT("USaveGameSubsystem")}) {
}

void USaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
	Logger.Log(TEXT("Initialize"));
}

void USaveGameSubsystem::Deinitialize() {
	Super::Deinitialize();
	Logger.Log(TEXT("Deinitialize"));
}

UPlayerSaveGame* USaveGameSubsystem::CreatePlayerGameSave(const FString& SlotName, int32 SlotIndex) const {
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass());
	UPlayerSaveGame* PlayerSaveGame = Cast<UPlayerSaveGame>(SaveGameObject);
	PlayerSaveGame->SlotName = SlotName;
	PlayerSaveGame->SlotIndex = SlotIndex;
	return PlayerSaveGame;;
}

UWorldSaveGame* USaveGameSubsystem::CreateWorldGameSave() const {
	return Cast<UWorldSaveGame>(UGameplayStatics::CreateSaveGameObject(UWorldSaveGame::StaticClass()));
}

UPlayerSaveGame* USaveGameSubsystem::GetPlayerSaveGame(const FString& SlotName, int32 SlotIndex) const {
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
		return nullptr;

	return Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex));
}

bool USaveGameSubsystem::SetPlayerSaveGame(UPlayerSaveGame* SaveGame) const {
	// 先删除再保存
	if (UGameplayStatics::DoesSaveGameExist(SaveGame->SlotName, SaveGame->SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SaveGame->SlotName, SaveGame->SlotIndex);
	}
	
	return UGameplayStatics::SaveGameToSlot(SaveGame, SaveGame->SlotName, SaveGame->SlotIndex);
}

bool USaveGameSubsystem::DeletePlayerSaveGame(const FString& SlotName, int32 SlotIndex) {
	return UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
}

bool USaveGameSubsystem::DeleteWorldSaveGame(const FString& SlotName, int32 SlotIndex) {
	return UGameplayStatics::DeleteGameInSlot(GetRealWorldSaveGameSlotName(SlotName), SlotIndex);
}

UPlayerSaveGame* USaveGameSubsystem::GetLoadedPlayerSaveGame() const {
	return GetPlayerSaveGame(LoadSlotName, LoadSlotIndex);
}

UWorldSaveGame* USaveGameSubsystem::GetLoadedWorldSaveGame() const {
	return GetWorldSaveGame(LoadSlotName, LoadSlotIndex);
}

bool USaveGameSubsystem::GetAndLoadWorldSaveGame(UWorld* MapToLoaded) const {
	return GetAndLoadWorldSaveGame(DefaultWorldSaveGameSlotName, DefaultWorldSaveGameSlotIndex, MapToLoaded);
}

bool USaveGameSubsystem::GetAndLoadWorldSaveGame(const FString& SlotName, int32 SlotIndex, UWorld* MapToLoaded) const {
	FString WorldSlotName = GetRealWorldSaveGameSlotName(SlotName);
	int32 WorldSlotIndex = SlotIndex;
	if (!UGameplayStatics::DoesSaveGameExist(WorldSlotName, WorldSlotIndex))
		return false;

	if (!MapToLoaded) 
		return false;
	
	FString WorldName = MapToLoaded->GetName();
	WorldName.RemoveFromStart(MapToLoaded->StreamingLevelsPrefix);
	UWorldSaveGame* SaveGame = Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot(WorldSlotName, WorldSlotIndex));
	if (!SaveGame || !SaveGame->HasMap(WorldName))
		return false;
	
	// 新建地图存档，并保存地图数据
	FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);

	for (const FSavedActor& SavedActor : SavedMap.SavedActors)
	{
		TArray<AActor*> WorldActors;
		UGameplayStatics::GetAllActorsWithInterface(MapToLoaded, USaveInterface::StaticClass(), WorldActors);
		for (AActor* WorldActor : WorldActors)
		{
			if (WorldActor->GetFName() == SavedActor.ActorName)
			{
				if (ISaveInterface::Execute_ShouldLoadTransform(WorldActor))
				{
					WorldActor->SetActorTransform(SavedActor.Transform);
				}
					
				FMemoryReader MemoryReader(SavedActor.Bytes);
				FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
				Archive.ArIsSaveGame = true;
					
				WorldActor->Serialize(Archive);

				ISaveInterface::Execute_LoadActor(WorldActor);
			}
		} 
	}

	return true;
}

UWorldSaveGame* USaveGameSubsystem::GetWorldSaveGame() const {
	return GetWorldSaveGame(DefaultWorldSaveGameSlotName, DefaultWorldSaveGameSlotIndex);
}

UWorldSaveGame* USaveGameSubsystem::GetWorldSaveGame(const FString& SlotName, int32 SlotIndex) const {
	FString WorldSlotName = GetRealWorldSaveGameSlotName(SlotName);
	int32 WorldSlotIndex = SlotIndex;
	return Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot(WorldSlotName, WorldSlotIndex));
}

bool USaveGameSubsystem::SetWorldSaveGame(UWorld* MapToSaved) const {
	return SetWorldSaveGame(DefaultWorldSaveGameSlotName, DefaultWorldSaveGameSlotIndex, MapToSaved);
}

bool USaveGameSubsystem::SetWorldSaveGame(const FString& SlotName, int32 SlotIndex, UWorld* MapToSaved) const {
	FString WorldSlotName = GetRealWorldSaveGameSlotName(SlotName);
	int32 WorldSlotIndex = SlotIndex;
	FString WorldName = MapToSaved->GetName();
	WorldName.RemoveFromStart(MapToSaved->StreamingLevelsPrefix);
	
	UWorldSaveGame* SaveGame = GetWorldSaveGame(SlotName, SlotIndex);
	if (!SaveGame)
		SaveGame = CreateWorldGameSave();
	
	if (!SaveGame->HasMap(WorldName))
	{
		FSavedMap SavedMap;
		SavedMap.MapAssetName = WorldName;
		SaveGame->SavedMaps.Add(SavedMap);
	}
	
	// 新建地图存档，并保存地图数据
	FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
	SavedMap.SavedActors.Empty();
	for (FActorIterator It(MapToSaved); It; ++It)
	{
		AActor* Actor = *It;
		
		if (!IsValid(Actor) || !Actor->Implements<USaveInterface>())
		{
			continue;	
		}
		
		FSavedActor SavedActor;
		SavedActor.ActorName = Actor->GetFName();
		SavedActor.Transform = Actor->GetTransform();

		FMemoryWriter MemoryWriter(SavedActor.Bytes);

		FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
		Archive.ArIsSaveGame = true;
			
		Actor->Serialize(Archive);

		// 内部是通过 == 操作来比较的
		SavedMap.SavedActors.AddUnique(SavedActor);
	}

	// 替换旧的地图存档
	for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
	{
		if (MapToReplace.MapAssetName == WorldName)
		{
			MapToReplace = SavedMap;
		}
	}

	// 保存
	UGameplayStatics::SaveGameToSlot(SaveGame, WorldSlotName, WorldSlotIndex);
	return true;
}


FString USaveGameSubsystem::GetRealWorldSaveGameSlotName(const FString& SlotName) const {
	return TEXT("World_") + SlotName;
}
