// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArcaneShards.h"


FString UArcaneShards::GetDescription(int32 InLevel)
{
	int32 ShardCount = FMath::Min(InLevel, MaxShardsCount);
	
	FString ExtraDesc = FString::Printf(
		TEXT("<Default>在地面生成</><Special>%d</><Default>个向上突起的地刺，造成范围伤害: </><Damage>%d</>"),
		ShardCount,
		static_cast<int>(GetDamageAtLevel(InLevel)));
	
	return GetDescTemplate(InLevel, 
		TEXT("奥术地刺"),
		ExtraDesc);
}

FString UArcaneShards::GetNextLevelDescription(int32 InLevel)
{
	int32 NextLevel = InLevel + 1;
	
	int32 CurrShardCount = FMath::Min(InLevel, MaxShardsCount);
	int32 NextShardCount = FMath::Min(NextLevel, MaxShardsCount);
	
	FString ExtraDesc = FString::Printf(TEXT("<Default>地刺数量：</><Special>%d</> -> <Special>%d</>\n"),
			CurrShardCount,
			NextShardCount);
	
	return GetNextLevelDescTemplate(InLevel, ExtraDesc);
}

FString UArcaneShards::GetLockedDescription()
{
	return Super::GetLockedDescription();
}
