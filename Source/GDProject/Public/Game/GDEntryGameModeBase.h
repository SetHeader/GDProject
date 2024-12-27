// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GDEntryGameModeBase.generated.h"

/**
 * 客户端入口界面的游戏模式
 * 用于查找房间、加入房间
 */
UCLASS(BlueprintType, Blueprintable)
class GDPROJECT_API AGDEntryGameModeBase : public AGameModeBase {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	// 进入地图
	void TravelToMap(const FString& MapName);

public:
	UPROPERTY(EditDefaultsOnly, Category = "AGDEntryGameModeBase")
	FString DefaultUserName = "Aura";
	
	UPROPERTY(EditDefaultsOnly, Category = "AGDEntryGameModeBase")
	FString DefaultMapName;
	
	UPROPERTY(EditDefaultsOnly, Category = "AGDEntryGameModeBase")
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly, Category = "AGDEntryGameModeBase")
	FName DefaultPlayerStartTag;
	
	// 地图名字和直接的地图是分开的，可以通过地图名来查找地图
	// 地图名 -> 地图。
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
};
