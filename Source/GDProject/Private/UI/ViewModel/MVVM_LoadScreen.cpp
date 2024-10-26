// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/GDGameModeBase.h"
#include "Game/GDGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	// 仅指定存档槽的名字和下标，详细数据需要在LoadData中加载
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AGDGameModeBase* GameModeBase = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	// 多人模式中没有GameModeBase
	if (!IsValid(GameModeBase))
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, FString("Please switch to Single Player"));
		return;
	}

	LoadSlots[Slot]->SetMapName(GameModeBase->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->PlayerStartTag = GameModeBase->DefaultPlayerStartTag;
	GameModeBase->SaveSlotData(LoadSlots[Slot], Slot);
	
	LoadSlots[Slot]->InitializeSlot();

	UGDGameInstance* GameInstance = Cast<UGDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GameInstance->LoadSlotIndex = Slot;
	GameInstance->LoadSlotName = LoadSlots[Slot]->LoadSlotName;
	GameInstance->PlayerStartTag = GameModeBase->DefaultPlayerStartTag;
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (TTuple<int, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (SelectedSlot)
	{
		AGDGameModeBase::DeleteSlot(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AGDGameModeBase* GameModeBase = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	UGDGameInstance* GameInst = Cast<UGDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	// 多人模式中没有GameModeBase
	if (!IsValid(GameModeBase))
	{
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, FString("Please switch to Single Player"));
		return;
	}
	
	GameInst->PlayerStartTag = SelectedSlot->PlayerStartTag;
	GameInst->LoadSlotName = SelectedSlot->LoadSlotName;
	GameInst->LoadSlotIndex = SelectedSlot->SlotIndex;
	
	GameModeBase->DefaultPlayerStartTag = GameInst->PlayerStartTag;
	
	if (IsValid(SelectedSlot)) {
		GameModeBase->TravelToMap(SelectedSlot);
	}
}

void UMVVM_LoadScreen::LoadData()
{
	AGameModeBase* GMB = UGameplayStatics::GetGameMode(this);
	AGDGameModeBase* GameModeBase = Cast<AGDGameModeBase>(GMB);
	if (!GameModeBase) return;

	// 把SaveGame存档数据转移到对应UMVVM_LoadSlot插槽中。
	for (TTuple<int, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = GameModeBase->GetSaveSlotData(LoadSlot.Value->LoadSlotName, LoadSlot.Key);
		
		const FString PlayerName = LoadSlot.Value->GetPlayerName();

		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SlotStatus = SaveObject->SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(SaveObject->PlayerName);
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->InitializeSlot();
	}
}
