// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class USimpleUserWidget;
class UMVVM_LoadScreen;
class ULoadScreenWidget;

UCLASS(BlueprintType, Blueprintable)
class GDPROJECT_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USimpleUserWidget> MainMenuWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USimpleUserWidget> MainMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> MainMenuViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> MainMenuViewModel;
	
protected:
	virtual void BeginPlay() override;
};
