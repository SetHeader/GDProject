// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WidgetController.generated.h"

class UEquipmentManagerComponent;
class UInventoryManagerComponent;
class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GDPROJECT_API UWidgetController : public UObject {
	GENERATED_BODY()

public:
	virtual void Initialize(APlayerController* Owner);
	virtual void Deinitialize();
	
protected:
	virtual void OnInitialized() {}

	virtual void OnDeinitialized() {}

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<ULocalPlayer> LocalPlayer;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<const UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<ACharacter> Character;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TWeakObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
};
