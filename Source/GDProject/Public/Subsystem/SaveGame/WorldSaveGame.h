// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WorldSaveGame.generated.h"

class UGameplayAbility;

// 存档地图中的Actor信息
USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY()
	FName ActorName = FName();

	UPROPERTY()
	FTransform Transform = FTransform();

	// Serialized variables from the Actor - only those marked with SaveGame specifier
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

// 存档地图信息
USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString MapAssetName = FString();

	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};


/**
 * 具体的存档数据
 */
UCLASS()
class GDPROJECT_API UWorldSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/* Maps */
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;
	
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
};
