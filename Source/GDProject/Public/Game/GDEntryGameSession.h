// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "SimpleLogger.h"
#include "GDEntryGameSession.generated.h"

USTRUCT(BlueprintType)
struct FUISessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 SearchResultIndex = 0;
	UPROPERTY(BlueprintReadOnly)
	FString SessionName;
	UPROPERTY(BlueprintReadOnly)
	FString GameType;
	UPROPERTY(BlueprintReadOnly)
	FString MapName;
	UPROPERTY(BlueprintReadOnly)
	int32 PlayerNum = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 PlayerMaxNum = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Ping = -1;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSessionFound, const TArray<FUISessionInfo>&)

UCLASS()
class GDPROJECT_API AGDEntryGameSession : public AGameSession {
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	// 查找Session
	void FindSession();
	// 查找完成
	void OnFindSessionsComplete(bool bWasSuccessful);
	
	// 加入Session
	bool JoinSession(const FUniqueNetIdPtr& UserId, int32 SessionIndexInSearchResults);
	// 加入完成
	void OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result);
	// 方便获取OnlineSessionPtr
	IOnlineSessionPtr GetOnlineSessionPtr() const;
	
public:
	FOnSessionFound OnSessionFoundDelegate;
	
private:
	SimpleLogger Logger = SimpleLogger(TEXT("AGDEntryGameSession"), SimpleLogger::CreateNetworkInfoFormatter(this));
	
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	// 缓存查找结果
	TSharedPtr<FOnlineSessionSearch> SearchSetting;
};
