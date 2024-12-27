// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/GDMainMenuWidgetController.h"

#include "Blueprint/UserWidget.h"
#include "Game/GDEntryGameModeBase.h"
#include "Game/GDEntryGameSession.h"
#include "GDProject/GDProject.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystem/SaveGame/PlayerSaveGame.h"
#include "Subsystem/SaveGame/SaveGameSubsystem.h"

void UGDMainMenuWidgetController::OnInitialized() {
	Super::OnInitialized();

	SaveGameSubsystem = UGameInstance::GetSubsystem<USaveGameSubsystem>(UGameplayStatics::GetGameInstance(this));
}

void UGDMainMenuWidgetController::BeginDestroy() {
	Super::BeginDestroy();
	AGDEntryGameModeBase* GameModeBase = Cast<AGDEntryGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeBase) 
		return;
	
	AGDEntryGameSession* GameSession = Cast<AGDEntryGameSession>(GameModeBase->GameSession);
	if (!GameSession) 
		return;
	
	GameSession->OnSessionFoundDelegate.Remove(FindSessionHandle);
}

void UGDMainMenuWidgetController::RouteTo(TSubclassOf<UUserWidget> UserWidgetClass) {
	if (!UserWidgetClass || !PlayerController.IsValid())
		return;
	
	if (!WidgetStack.IsEmpty())
		WidgetStack.Top()->RemoveFromParent();
	
	UUserWidget* UserWidget = CreateWidget<UUserWidget>(PlayerController.Get(), UserWidgetClass);
	WidgetStack.Emplace(UserWidget);
	UserWidget->AddToViewport();
}

void UGDMainMenuWidgetController::RouteBack() {
	if (!WidgetStack.IsEmpty())
		WidgetStack.Pop()->RemoveFromParent();

	if (!WidgetStack.IsEmpty())
		WidgetStack.Top()->AddToViewport();
}

void UGDMainMenuWidgetController::RouteBackToFirst() {
	while (WidgetStack.Num() > 1)
		RouteBack();
}

void UGDMainMenuWidgetController::RerouteTo(TSubclassOf<UUserWidget> UserWidgetClass) {
	RouteBack();
	RouteTo(UserWidgetClass);
}

void UGDMainMenuWidgetController::PlayGameInLocal() {
	AGDEntryGameModeBase* GameModeBase = Cast<AGDEntryGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeBase) {
		GDLog::ErrorStatic(TEXT("UGDMainMenuWidgetController"), TEXT("FindSession Error, GameModeBase is null"));
		return;
	}

	if (!SaveGameSubsystem) {
		GDLog::ErrorStatic(TEXT("UGDMainMenuWidgetController"), TEXT("SaveGameSubsystem Not Found"));
		return;
	}
	
	UPlayerSaveGame* PlayerSaveGame = SaveGameSubsystem->GetLoadedPlayerSaveGame();
	if (PlayerSaveGame) {
		GameModeBase->TravelToMap(PlayerSaveGame->MapName);
	}
}

void UGDMainMenuWidgetController::FindSession() {
	AGDEntryGameModeBase* GameModeBase = Cast<AGDEntryGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeBase) {
		GDLog::ErrorStatic(TEXT("UGDMainMenuWidgetController"), TEXT("FindSession Error, GameModeBase is null"));
		return;
	}
	
	AGDEntryGameSession* GameSession = Cast<AGDEntryGameSession>(GameModeBase->GameSession);
	if (!GameSession) {
		GDLog::ErrorStatic(TEXT("UGDMainMenuWidgetController"), TEXT("FindSession Error, GameSession is null"));
		return;
	}
	if (!FindSessionHandle.IsValid()) {
		FindSessionHandle = GameSession->OnSessionFoundDelegate.AddLambda([this](const TArray<FUISessionInfo>& SessionInfos) {
			OnSessionFoundDelegate.Broadcast(SessionInfos);
		});
	}

	GameSession->FindSession();
}

bool UGDMainMenuWidgetController::JoinSession() {
	int32 SearchResultIndex = SelectedSession.SearchResultIndex;
	AGDEntryGameModeBase* GameModeBase = Cast<AGDEntryGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameModeBase) {
		GDLog::ErrorStatic(TEXT("UGDMainMenuWidgetController"), TEXT("FindSession Error, GameModeBase is null"));
		return false;
	}
	
	AGDEntryGameSession* GameSession = Cast<AGDEntryGameSession>(GameModeBase->GameSession);
	if (!GameSession) {
		GDLog::ErrorStatic(TEXT("UGDMainMenuWidgetController"), TEXT("FindSession Error, GameSession is null"));
		return false;
	}

	if (!LocalPlayer.IsValid()) {
		GDLog::ErrorStatic(TEXT("UGDMainMenuWidgetController"), TEXT("FindSession Error, LocalPlayer is invalid"));
		return false;
	}
	const auto ID = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
	GameSession->JoinSession(ID, SearchResultIndex);
	return true;
}

void UGDMainMenuWidgetController::SetSelectedSessionInfo(FUISessionInfo SessionInfo) {
	SelectedSession = SessionInfo;
	OnSessionInfoChangedDelegate.Broadcast();
}
