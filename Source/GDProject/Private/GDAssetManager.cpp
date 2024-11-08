// Fill out your copyright notice in the Description page of Project Settings.


#include "GDAssetManager.h"
#include <GDGameplayTags.h>

UGDAssetManager* UGDAssetManager::INSTANCE;

UGDAssetManager& UGDAssetManager::Get()
{
	if (!INSTANCE) {
		INSTANCE = Cast<UGDAssetManager>(GEngine->AssetManager);
	}
	return *Cast<UGDAssetManager>(GEngine->AssetManager);
}

void UGDAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
	FGDGameplayTags::Get().InitializeGameplayTags();
	// 使用TargetData需要调用该函数初始化！ 注意：UE 5.4 不需要调用，引擎已经调用过了
	// UAbilitySystemGlobals::Get().InitGlobalData();
}
