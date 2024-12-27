// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleUserWidget.generated.h"

class UUISubsystem;
class UWidgetController;
class UWidgetViewModel;

/**
 * 
 */
UCLASS()
class GDPROJECT_API USimpleUserWidget : public UUserWidget {
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	virtual void BeginDestroy() override;

	void OnUISubsystemInited(UUISubsystem* InSubsystem);

	UFUNCTION(BlueprintGetter, Category = "SimpleUserWidget")
	UUISubsystem* GetUISubsystem() const;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetViewModelSet();

	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerOrViewModelSet();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "SimpleUserWidget")
	TSubclassOf<UWidgetController> WidgetControllerClass;

	UPROPERTY(BlueprintReadOnly, Category = "SimpleUserWidget")
	TObjectPtr<UWidgetController> WidgetController;

	UPROPERTY(EditDefaultsOnly, DisplayName = "AlwaysCreateNewWidgetController", Category = "SimpleUserWidget")
	bool bAlwaysCreateNewWidgetController = false;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleUserWidget")
	TSubclassOf<UWidgetViewModel> WidgetViewModelClass;
	
	UPROPERTY(BlueprintReadOnly, Category = "SimpleUserWidget")
	TObjectPtr<UWidgetViewModel> WidgetViewModel;

	UPROPERTY(EditDefaultsOnly, DisplayName = "AlwaysCreateNewWidgetViewModel", Category = "SimpleUserWidget")
	bool bAlwaysCreateNewWidgetViewModel = false;
};
