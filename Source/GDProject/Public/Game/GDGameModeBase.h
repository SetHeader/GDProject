// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GDGameModeBase.generated.h"

class UMVVM_LoadSlot;
class ULoadScreenSaveGame;
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
	// 配置敌人和主角的默认属性、默认能力
	UPROPERTY(EditDefaultsOnly, Category="GDGameModeBase")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	// 配置主角的所有技能定义
	UPROPERTY(EditDefaultsOnly, Category="GDGameModeBase")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULoadScreenSaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	// 地图名字和直接的地图是分开的，可以通过地图名来查找地图
	// 地图名 -> 地图。
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
	
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;
	
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;
	
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	// 查找当前存档的数据
	ULoadScreenSaveGame* RetrieveInGameSaveData() const;
	// 保存数据到当前存档
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	// 保存世界的状态
	void SaveWorldState(UWorld* World) const;
	void LoadWorldState(UWorld* World) const;
	
	// 进入存档中的地图
	void TravelToMap(UMVVM_LoadSlot* Slot);
	
	// 选择玩家出生点
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
protected:
	virtual void BeginPlay() override;
};