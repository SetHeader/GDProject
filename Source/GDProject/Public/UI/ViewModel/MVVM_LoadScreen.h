// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Base/WidgetViewModel.h"
#include "MVVM_LoadScreen.generated.h"

class UGDMainMenuWidgetController;
class UMVVM_LoadSlot;
class USaveGameSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayButtonClicked);

/**
 * 
 */
UCLASS()
class GDPROJECT_API UMVVM_LoadScreen : public UWidgetViewModel
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPlayButtonClicked OnPlayButtonClickedDelegate;
	
private:
	UPROPERTY()
	TObjectPtr<USaveGameSubsystem> SaveGameSubsystem;
	
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;
	
	// 当前选择的存档槽
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> SelectedSlot;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;
	
	// 存档插槽下标 -> 对应插槽VM
	UPROPERTY()
	TMap<int32, TObjectPtr<UMVVM_LoadSlot>> LoadSlots;

	UPROPERTY()
	TObjectPtr<UGDMainMenuWidgetController> MainMenuWidgetController;
	
public:
	virtual void OnInitialized() override;

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	// 新建游戏
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);
	// 新建存档
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 SlotIndex, const FString& PlayerName);
	// 选择存档进入游戏
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

private:
	USaveGameSubsystem* GetSaveGameSubsystem() const;
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
};
