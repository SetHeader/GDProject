// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SaveGame/WorldSaveGame.h"

FSavedMap UWorldSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (const FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapAssetName == InMapName)
		{
			return SavedMap;
		}
	}
	
	return FSavedMap();
}

bool UWorldSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapAssetName == InMapName)
		{
			return true;
		}
	}
	
	return false;
}
