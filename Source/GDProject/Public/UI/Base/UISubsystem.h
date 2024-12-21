// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UISubsystem.generated.h"

class UWidgetController;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUISubsystemInitedSignature, UUISubsystem* /*this*/)
/**
 * 
 */
UCLASS()
class GDPROJECT_API UUISubsystem : public ULocalPlayerSubsystem {
	GENERATED_BODY()

public:
	// 需要客户端调用初始化
	void InitializeSubsystem();
	
	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;
	
	UWidgetController* FindOrCreateWidgetController(TSubclassOf<UWidgetController> WidgetControllerClass);
	
	UWidgetController* CreateWidgetController(TSubclassOf<UWidgetController> WidgetControllerClass) const;

	bool IsInitialized() const { return bInitialized;	}
public:
	FOnUISubsystemInitedSignature OnInitedDelegate;
private:
	UPROPERTY()
	TMap<TSubclassOf<UWidgetController>, TObjectPtr<UWidgetController>> WidgetControllers;
	bool bInitialized = false;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> CurrentPlayerController;
};
