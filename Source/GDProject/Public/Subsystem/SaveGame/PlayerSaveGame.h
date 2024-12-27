// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

class UGameplayAbility;

// 存档槽状态
UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,		// 空缺
	EnterName,	// 新建存档，输入名称中
	Taken		// 已占用
};

// 存档能力的结构
USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();
	
	// 状态标签，对应 Abilities_Status_xxx
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();

	// 输入标签，对应 InputTag_xxx
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();

	// 类型标签，对应 Abilities_Type_xxx 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel = 0;
	
};

/**
 * 玩家存档
 */
UCLASS()
class GDPROJECT_API UPlayerSaveGame : public USaveGame {
	GENERATED_BODY()

public:
	// 存档插槽名
	UPROPERTY()
	FString SlotName = FString();
	// 存档插槽下标
	UPROPERTY()
	int32 SlotIndex = 0;
	// 插槽状态
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;
	
	UPROPERTY()
	FString PlayerName = FString("Default Name");
	// 玩家所在的地图名
	UPROPERTY()
	FString MapName = FString("Default Map Name");
	// 在地图中的出生点
	UPROPERTY()
	FName PlayerStartTag;
	
	// 是否是第一次加载
	UPROPERTY()
	bool bFirstTimeLoadIn = true;
	
	/* Player */
	
	UPROPERTY()
	int32 PlayerLevel = 0;

	UPROPERTY()
	int32 XP = 0;

	UPROPERTY()
	int32 SpellPoints = 0;

	UPROPERTY()
	int32 AttributePoints = 0;

	UPROPERTY()
	float Strength = 0;

	UPROPERTY()
	float Intelligence = 0;

	UPROPERTY()
	float Resilience = 0;

	UPROPERTY()
	float Vigor = 0;

	/* Abilities */
	
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;
};
