// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GDGameModeBase.generated.h"

class UMVVM_LoadSlot;
class UWorldSaveGame;
class UCharacterClassInfo;
class UAbilityInfo;
/**
 * 游戏模式
 */
UCLASS()
class GDPROJECT_API AGDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// 进入地图
	void TravelToMap(const FString& MapName);
	
	// 选择玩家出生点
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

private:
	UFUNCTION()
	void SaveWorld();

private:
	UPROPERTY(EditDefaultsOnly, Category = "AGDEntryGameModeBase")
	FString DefaultMapName;
	
	UPROPERTY(EditDefaultsOnly, Category = "AGDEntryGameModeBase")
	TSoftObjectPtr<UWorld> DefaultMap;
	// 地图名字和直接的地图是分开的，可以通过地图名来查找地图
	// 地图名 -> 地图。
	UPROPERTY(EditDefaultsOnly, Category = "AGDEntryGameModeBase")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	FTimerHandle SaveWorldTimerHandle;
};