// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GDGameModeBase.h"

#include "EngineUtils.h"
#include "Game/GDGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"


ULoadScreenSaveGame* AGDGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}

	return Cast<ULoadScreenSaveGame>(SaveGameObject);
}

void AGDGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	// 先删除再保存
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
	
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
}

void AGDGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* AGDGameModeBase::RetrieveInGameSaveData() const
{
	UGDGameInstance* GameInstance = Cast<UGDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	FString SlotName = GameInstance->LoadSlotName;
	int32 SlotIndex = GameInstance->LoadSlotIndex;
	return GetSaveSlotData(SlotName, SlotIndex);
}

void AGDGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UGDGameInstance* AuraGameInstance = Cast<UGDGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

void AGDGameModeBase::SaveWorldState(UWorld* World) const
{
	FString WorldName = World->GetName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UGDGameInstance* GI = Cast<UGDGameInstance>(GetGameInstance());
	check(GI);

	if (ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData())
	{
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap SavedMap;
			SavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(SavedMap);
		}

		// 新建地图存档，并保存地图数据
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();
		for (FActorIterator It(World); It; ++It)
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
		UGameplayStatics::SaveGameToSlot(SaveGame, GI->LoadSlotName, GI->LoadSlotIndex);
	}
	
}

void AGDGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UGDGameInstance* GI = Cast<UGDGameInstance>(GetGameInstance());
	check(GI);

	if (ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData())
	{
		if (!SaveGame->HasMap(WorldName))
		{
			return;
		}

		// 新建地图存档，并保存地图数据
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);

		for (const FSavedActor& SavedActor : SavedMap.SavedActors)
		{
			TArray<AActor*> WorldActors;
			UGameplayStatics::GetAllActorsWithInterface(World, USaveInterface::StaticClass(), WorldActors);
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

		// 保存
		UGameplayStatics::SaveGameToSlot(SaveGame, GI->LoadSlotName, GI->LoadSlotIndex);
	}
}

void AGDGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* AGDGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UGDGameInstance* GameInstance = Cast<UGDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	for (AActor* Actor : Actors)
	{
		APlayerStart* PlayerStart = CastChecked<APlayerStart>(Actor);
		if (PlayerStart->PlayerStartTag == GameInstance->PlayerStartTag)
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
}
