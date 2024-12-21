// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleUserWidget.generated.h"

class UUISubsystem;
class UWidgetController;

/**
 * 
 */
UCLASS()
class GDPROJECT_API USimpleUserWidget : public UUserWidget {
	GENERATED_BODY()

	friend class UUISubsystem;

	virtual bool Initialize() override;

	virtual void BeginDestroy() override;

	void OnUISubsystemInited(UUISubsystem* InSubsystem);

	UFUNCTION(BlueprintGetter, Category = "SimpleUserWidget")
	UUISubsystem* GetUISubsystem() const;
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "SimpleUserWidget")
	TObjectPtr<UWidgetController> WidgetController;
	
	UPROPERTY(EditDefaultsOnly, Category = "SimpleUserWidget")
	TSubclassOf<UWidgetController> WidgetControllerClass;
	
	UPROPERTY(EditDefaultsOnly, DisplayName = "AlwaysCreateNewWidgetController", Category = "SimpleUserWidget")
	bool bAlwaysCreateNewWidgetController = false;
};
