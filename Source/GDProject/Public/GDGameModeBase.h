// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GDGameModeBase.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class GDPROJECT_API AGDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY(EditDefaultsOnly, Category="GDGameModeBase")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
};
