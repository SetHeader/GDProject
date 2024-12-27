// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleLogger.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveGameSubsystem.generated.h"

class UWorldSaveGame;
class UPlayerSaveGame;
/**
 * 存档系统，可以在单人、多人、监听服务器上动作。
 * 存档分为世界存档和玩家存档。世界存档保存世界的状态，所有玩家共享；玩家存档则保存当前玩家的信息。
 * Standalone：世界存档、玩家存档都保存在本地。世界存档与玩家存档可以有多个，且一个玩家存档对应一个世界存档。
 * DelicatedServer：只保存世界存档。且永远只有一个世界存档。
 * ListenServer：保存世界存档和本地玩家的存档。世界存档与本地玩家存档可以有多个，且一个本地玩家存档对应一个世界存档。
 * Client：保存本地玩家存档。可以有多个本地玩家存档。
 */
UCLASS()
class GDPROJECT_API USaveGameSubsystem : public UGameInstanceSubsystem {
	GENERATED_BODY()
	
public:
	USaveGameSubsystem();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	UPlayerSaveGame* CreatePlayerGameSave(const FString& SlotName, int32 SlotIndex) const;
	UWorldSaveGame* CreateWorldGameSave() const;
	
	// 读取玩家存档
	UPlayerSaveGame* GetPlayerSaveGame(const FString& SlotName, int32 SlotIndex) const;
	// 保存玩家存档
	bool SetPlayerSaveGame(UPlayerSaveGame* SaveGame) const;
	// 删除玩家存档
	bool DeletePlayerSaveGame(const FString& SlotName, int32 SlotIndex);
	
	// 服务端读取默认的存档，并加载到指定地图上。
	bool GetAndLoadWorldSaveGame(UWorld* MapToLoaded) const;
	// 服务端读取指定的存档，并加载到指定地图上。
	bool GetAndLoadWorldSaveGame(const FString& SlotName, int32 SlotIndex, UWorld* MapToLoaded) const;
	
	// 服务端读取默认的存档
	UWorldSaveGame* GetWorldSaveGame() const;
	// 服务端读取指定的存档
	UWorldSaveGame* GetWorldSaveGame(const FString& SlotName, int32 SlotIndex) const;
	
	// 服务端存档某个地图到默认插槽
	bool SetWorldSaveGame(UWorld* MapToSaved) const;
	// 服务端存档某个地图到指定插槽
	bool SetWorldSaveGame(const FString& SlotName, int32 SlotIndex, UWorld* MapToSaved) const;
	bool DeleteWorldSaveGame(const FString& SlotName, int32 SlotIndex);
	
	UPlayerSaveGame* GetLoadedPlayerSaveGame() const;
	UWorldSaveGame* GetLoadedWorldSaveGame() const;

private:
	FString GetRealWorldSaveGameSlotName(const FString& SlotName) const;
	
	
public:
	// 当前玩家的起点位置
	UPROPERTY()
	FName PlayerStartTag = FName();
	
	// 当前加载的存档槽名字
	UPROPERTY()
	FString LoadSlotName = FString();
	
	// 当前加载的存档槽下标
	UPROPERTY()
	int32 LoadSlotIndex = 0;
	
private:
	SimpleLogger Logger = SimpleLogger(TEXT("USaveGameSubsystem"));

	FString DefaultWorldSaveGameSlotName = TEXT("Default_World_SaveGame");
	int32 DefaultWorldSaveGameSlotIndex = 0;
};