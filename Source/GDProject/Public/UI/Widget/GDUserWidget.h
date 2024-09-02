// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GDUserWidget.generated.h"

class UGDWidgetController;

/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UGDWidgetController> WidgetController;
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UGDWidgetController* InWidgetController);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnWidgetControllerSet();
	
};
