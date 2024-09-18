// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT()
struct FGDLevelUpInfo
{
	GENERATED_BODY()
	// 这是升级到下一级所需要的经验，不是升级到这一级需要的经验
	// 即表示当前等级的经验条上限。到达上限后进入下一级
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

UCLASS()
class GDPROJECT_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCurveTable> LevelUpTable;

	// 升级信息，如 LevelUpInfos[1] 表示升到 1级的信息，LevelUpInfos[2] 表示升到 2级的信息。
	// LevelUpInfos[0]没有含义，仅作为占位符
	UPROPERTY(EditDefaultsOnly)
	TArray<FGDLevelUpInfo> LevelUpInfos;

	/**
	 * 通过累计经验值来获取对应等级
	 * @param XP 累计经验值
	 * @return 对应等级
	 */
	int32 FindLevelForXP(int32 XP);
};
