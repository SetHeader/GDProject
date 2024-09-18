// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
	// LevelUpInfo[1] 表示升到 1级的经验条长度，经验条满了就进入下一级
	// LevelUpInfo[0]没有含义，仅作为占位符
	int32 Level = 1;

	bool bSearing = true;
	while (bSearing)
	{
		if (Level < LevelUpInfos.Num())
		{
			if (XP < LevelUpInfos[Level].LevelUpRequirement)
			{
				bSearing = false;
			} else
			{
				Level++;
			}
		}
	}

	return Level;
}
