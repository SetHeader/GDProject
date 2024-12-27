// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleLogger.h"
#include "GameFramework/GameSession.h"
#include "GDGameSession.generated.h"

UCLASS()
class GDPROJECT_API AGDGameSession : public AGameSession {
	GENERATED_BODY()

	
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void RegisterServer() override;
	virtual void RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite) override;

	bool CreateSession();
	void OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful);

private:
	SimpleLogger Logger = SimpleLogger(TEXT("AGDGameSession"), SimpleLogger::CreateNetworkInfoFormatter(this));
	
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
};
