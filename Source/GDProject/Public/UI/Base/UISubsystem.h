// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformCrashContext.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UISubsystem.generated.h"

class UWidgetViewModel;
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
	UFUNCTION(BlueprintCallable, Category="UUISubsystem")
	UWidgetController* FindOrCreateWidgetController(TSubclassOf<UWidgetController> WidgetControllerClass);
	UFUNCTION(BlueprintCallable, Category="UUISubsystem")
	UWidgetController* CreateWidgetController(TSubclassOf<UWidgetController> WidgetControllerClass) const;
	UFUNCTION(BlueprintCallable, Category="UUISubsystem")
	UWidgetViewModel* FindOrCreateWidgetViewModel(TSubclassOf<UWidgetViewModel> WidgetViewModelClass);
	UFUNCTION(BlueprintCallable, Category="UUISubsystem")
	UWidgetViewModel* CreateWidgetViewModel(TSubclassOf<UWidgetViewModel> WidgetViewModelClass) const;

	bool IsInitialized() const { return bInitialized;	}
public:
	FOnUISubsystemInitedSignature OnInitedDelegate;
private:
	UPROPERTY()
	TMap<TSubclassOf<UWidgetController>, TObjectPtr<UWidgetController>> WidgetControllers;
	UPROPERTY()
	TMap<TSubclassOf<UWidgetViewModel>, TObjectPtr<UWidgetViewModel>> WidgetViewModels;
	bool bInitialized = false;

	UPROPERTY()
	TWeakObjectPtr<APlayerController> CurrentPlayerController;
};
