// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GDHUD.generated.h"

class UGDUserWidget;
class UAbilitySystemComponent;
class UAttributeSet;
class UGDWidgetController;
class UGDOverlayWidgetController;
class UGDAttributeMenuWidgetController;
class USpellMenuWidgetController;
/**
 * 
 */
UCLASS()
class GDPROJECT_API AGDHUD : public AHUD
{
	GENERATED_BODY()
	
	
private:
	UPROPERTY(EditAnywhere, Category = "GDHUD")
	TSubclassOf<UGDUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UGDUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere, Category = "GDHUD")
	TSubclassOf<UGDOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UGDOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere, Category = "GDHUD")
	TSubclassOf<UGDAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UGDAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere, Category = "GDHUD")
	TSubclassOf<UGDAttributeMenuWidgetController> SpellMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;
public:

	UGDOverlayWidgetController* GetOverlayWidgetController() const;

	UGDAttributeMenuWidgetController* GetAttributeMenuWidgetController() const;

	USpellMenuWidgetController* GetSpellMenuWidgetController() const;
	
	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);
};
