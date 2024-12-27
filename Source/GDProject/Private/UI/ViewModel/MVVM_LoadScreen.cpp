// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/GDEntryGameModeBase.h"
#include "Game/GDEntryGameSession.h"
#include "Game/GDGameModeBase.h"
#include "GDProject/GDProject.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/SaveGame/PlayerSaveGame.h"
#include "Subsystem/SaveGame/SaveGameSubsystem.h"
#include "UI/Base/UISubsystem.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "UI/WidgetController/GDMainMenuWidgetController.h"


void UMVVM_LoadScreen::OnInitialized()
{
	MainMenuWidgetController = Cast<UGDMainMenuWidgetController>(GetUISubsystem()->FindOrCreateWidgetController(UGDMainMenuWidgetController::StaticClass()));
	
	// 仅指定存档槽的名字和下标，详细数据需要在LoadData中加载
	for (int32 SlotIndex = 0; SlotIndex < 3; ++SlotIndex) {
		FString SlotName = FString("LoadSlot_") + FString::FromInt(SlotIndex);
		
		UMVVM_LoadSlot* LoadSlot = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
		UPlayerSaveGame* SaveObject = SaveGameSubsystem->GetPlayerSaveGame(SlotName, SlotIndex);
		
		LoadSlot->LoadSlotName = SlotName;
		LoadSlot->SlotIndex = SlotIndex;

		if (SaveObject) {
			const FString PlayerName = SaveObject->PlayerName;
			LoadSlot->PlayerStartTag = SaveObject->PlayerStartTag;
			LoadSlot->SlotStatus = SaveObject->SaveSlotStatus;
			LoadSlot->SetPlayerName(SaveObject->PlayerName);
			LoadSlot->SetMapName(SaveObject->MapName);
			LoadSlot->InitializeSlot();
		} else {
			LoadSlot->SlotStatus = Vacant;
		}
		
		LoadSlots.Add(SlotIndex, LoadSlot);
	}
	
	SaveGameSubsystem = GetSaveGameSubsystem();
	
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 SlotIndex, const FString& PlayerName)
{
	AGDEntryGameModeBase* GameModeBase = Cast<AGDEntryGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeBase) {
		GDLog::ErrorStatic(TEXT("UMVVM_LoadScreen"), TEXT("GameModeBase Not Found"));
		return;
	}
	
	LoadSlots[SlotIndex]->SetMapName(GameModeBase->DefaultMapName);
	if (PlayerName.IsEmpty()) {
		LoadSlots[SlotIndex]->SetPlayerName(GameModeBase->DefaultUserName);
	}
	LoadSlots[SlotIndex]->SlotStatus = Taken;
	LoadSlots[SlotIndex]->PlayerStartTag = GameModeBase->DefaultPlayerStartTag;
	
	SaveSlotData(LoadSlots[SlotIndex], SlotIndex);
	
	LoadSlots[SlotIndex]->InitializeSlot();
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 SlotIndex)
{
	SlotSelected.Broadcast();
	for (TTuple<int, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == SlotIndex)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
	}
	SelectedSlot = LoadSlots[SlotIndex];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (!SaveGameSubsystem) {
		GDLog::ErrorStatic(TEXT("UMVVM_LoadScreen"), TEXT("SaveGameSubsystem Not Found"));
		return;
	}
	
	if (SelectedSlot)
	{
		SaveGameSubsystem->DeletePlayerSaveGame(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	if (!SaveGameSubsystem) {
		GDLog::ErrorStatic(TEXT("UMVVM_LoadScreen"), TEXT("SaveGameSubsystem Not Found"));
		return;
	}
	
	if (!IsValid(SelectedSlot)) {
		GDLog::ErrorStatic(TEXT("UMVVM_LoadScreen"), TEXT("SelectedSlot Not Found"));
		return;
	}
	
	AGDEntryGameModeBase* GameModeBase = Cast<AGDEntryGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeBase) {
		GDLog::ErrorStatic(TEXT("UMVVM_LoadScreen"), TEXT("GameModeBase Not Found"));
		return;
	}
	
	SaveGameSubsystem->PlayerStartTag = SelectedSlot->PlayerStartTag;
	SaveGameSubsystem->LoadSlotName = SelectedSlot->LoadSlotName;
	SaveGameSubsystem->LoadSlotIndex = SelectedSlot->SlotIndex;
	
	OnPlayButtonClickedDelegate.Broadcast();
}

USaveGameSubsystem* UMVVM_LoadScreen::GetSaveGameSubsystem() const {
	return UGameInstance::GetSubsystem<USaveGameSubsystem>(UGameplayStatics::GetGameInstance(this));
}

void UMVVM_LoadScreen::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex) {
	// 先删除再保存
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}

	if (!SaveGameSubsystem) {
		GDLog::ErrorStatic(TEXT("UMVVM_LoadScreen"), TEXT("SaveGameSubsystem Not Found"));
		return;
	}
	USaveGame* SaveGameObject = SaveGameSubsystem->CreatePlayerGameSave(LoadSlot->LoadSlotName, SlotIndex);
	UPlayerSaveGame* SaveGame = Cast<UPlayerSaveGame>(SaveGameObject);
	SaveGame->PlayerName = LoadSlot->GetPlayerName();
	SaveGame->MapName = LoadSlot->GetMapName();
	SaveGame->SaveSlotStatus = Taken;
	SaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	SaveGameSubsystem->SetPlayerSaveGame(SaveGame);
}
