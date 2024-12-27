// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "WidgetViewModel.generated.h"

/**
 * 
 */
UCLASS()
class GDPROJECT_API UWidgetViewModel : public UMVVMViewModelBase {
	GENERATED_BODY()

public:
	virtual void Initialize(APlayerController* Owner);
	virtual void Deinitialize();
	
protected:
	virtual void OnInitialized() {}

	virtual void OnDeinitialized() {}

	UFUNCTION(BlueprintGetter, Category = "SimpleUserWidget")
	UUISubsystem* GetUISubsystem() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<ACharacter> Character;
};
