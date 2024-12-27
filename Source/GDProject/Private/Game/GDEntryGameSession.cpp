// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GDEntryGameSession.h"

#include "OnlineSubsystemUtils.h"
#include "GDProject/GDProject.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"
#include "Tools/StringTool.h"

void AGDEntryGameSession::BeginPlay()
{
	Super::BeginPlay();

	Logger.Log(TEXT("BeginPlay"));
}

void AGDEntryGameSession::FindSession()
{
	IOnlineSessionPtr SessionPtr = GetOnlineSessionPtr();
	if (!SessionPtr.IsValid())
		return;
	
	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!LocalPlayer)
		return;

	// 配置查找会话的参数，同时也存储查找结果，一般作为成员变量。类型是：TSharedPtr<FOnlineSessionSearch>
	SearchSetting = MakeShareable(new FOnlineSessionSearch());
	//SearchSetting->QuerySettings.Set(SEARCH_KEYWORDS, FString("Custom"), EOnlineComparisonOp::Equals);
	//SearchSetting->QuerySettings.Set(SEARCH_XBOX_LIVE_HOPPER_NAME, FString("TeamDeathmatch"), EOnlineComparisonOp::Equals);
	//SearchSetting->QuerySettings.Set(SEARCH_XBOX_LIVE_SESSION_TEMPLATE_NAME, FString("DemoGameSession"), EOnlineComparisonOp::Equals);
	SearchSetting->TimeoutInSeconds = 30.0f;
	SearchSetting->bIsLanQuery = true;
	
	// 监听查找会话是否完成
	OnFindSessionsCompleteDelegateHandle = SessionPtr->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &AGDEntryGameSession::OnFindSessionsComplete));
	
	// 获取UserId
	FUniqueNetIdPtr UserId = LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId();
	TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SearchSetting.ToSharedRef();
	
	// 开始查找会话
	SessionPtr->FindSessions(*UserId, SearchSettingsRef);
}

void AGDEntryGameSession::OnFindSessionsComplete(bool bWasSuccessful)
{
	IOnlineSessionPtr SessionPtr = GetOnlineSessionPtr();
	if (!SessionPtr.IsValid())
		return;
		
	// 清除监听
	SessionPtr->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
	
	// 查找完成通知
	TArray<FOnlineSessionSearchResult> ResultList = SearchSetting->SearchResults;
	
	TArray<FUISessionInfo> UISessionList;
	for (int32 SearchIdx = 0; SearchIdx < ResultList.Num(); SearchIdx++)
	{
		const FOnlineSessionSearchResult& SearchResult = ResultList[SearchIdx];
		FUISessionInfo UISession;
		UISession.SearchResultIndex = SearchIdx;
		UISession.SessionName = SearchResult.Session.OwningUserName;
		// 当前玩家数量
		UISession.PlayerNum = SearchResult.Session.SessionSettings.NumPublicConnections + SearchResult.Session.SessionSettings.NumPrivateConnections
			- SearchResult.Session.NumOpenPublicConnections - SearchResult.Session.NumOpenPrivateConnections;
		// 可容纳玩家数量
		UISession.PlayerMaxNum = SearchResult.Session.SessionSettings.NumPublicConnections + SearchResult.Session.SessionSettings.NumPrivateConnections;
		UISession.Ping = SearchResult.PingInMs;
		// 游戏类型
		SearchResult.Session.SessionSettings.Get(SETTING_GAMEMODE, UISession.GameType);
		// 地图名称
		SearchResult.Session.SessionSettings.Get(SETTING_MAPNAME, UISession.MapName);

		UISessionList.Emplace(UISession);
	}

	OnSessionFoundDelegate.Broadcast(UISessionList);
}

bool AGDEntryGameSession::JoinSession(const FUniqueNetIdPtr& UserId, int32 SessionIndexInSearchResults)
{
	IOnlineSessionPtr SessionPtr = GetOnlineSessionPtr();
	if (!SessionPtr.IsValid())
		return false;
	
	// 监听加入会话是否完成
	OnJoinSessionCompleteDelegateHandle = SessionPtr->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &AGDEntryGameSession::OnJoinSessionComplete));
	
	if (SessionIndexInSearchResults > -1 && SessionIndexInSearchResults < SearchSetting->SearchResults.Num())
	{
		const FOnlineSessionSearchResult& SearchResult = SearchSetting->SearchResults[SessionIndexInSearchResults];
		//异步
		bool Result = SessionPtr->JoinSession(*UserId, FName("NAME_GameSession2"), SearchResult);
		return Result;
	}
	
	return false;
}

void AGDEntryGameSession::OnJoinSessionComplete(FName InSessionName, EOnJoinSessionCompleteResult::Type Result)
{
	IOnlineSessionPtr SessionPtr = GetOnlineSessionPtr();
	if (!SessionPtr.IsValid())
		return;
	
	// 清除监听
	SessionPtr->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

	if (Result != EOnJoinSessionCompleteResult::Success)
		return;
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
		return;
	
	FString ConnectString;
	if (SessionPtr->GetResolvedConnectString(FName("NAME_GameSession2"), ConnectString))
	{
		// 从命令行中把玩家名称提前出来
		FString PlayerName = "ERROR";
		if (FParse::Value(FCommandLine::Get(), TEXT("PlayerName="), PlayerName))
		{
			// Succ
		}
		
		// 进入服务端的游戏
		PlayerController->ClientTravel(ConnectString + "?PN=" + PlayerName, TRAVEL_Absolute);

		Logger.Log(TEXT("OnJoinSessionComplete"));
	}
}

IOnlineSessionPtr AGDEntryGameSession::GetOnlineSessionPtr() const {
	if (IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld())) 
		return OnlineSub->GetSessionInterface();
	return nullptr;
}
