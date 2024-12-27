// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GDGameSession.h"
#include "Online.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSubsystemTypes.h"
#include "GDProject/GDProject.h"
#include "Interfaces/OnlineSessionDelegates.h"

void AGDGameSession::BeginPlay()
{
	Super::BeginPlay();
	
	Logger.Log(TEXT("BeginPlay"));
}

void AGDGameSession::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	Logger.Log(TEXT("EndPlay"));
}

void AGDGameSession::RegisterServer()
{
	Super::RegisterServer();

	Logger.Log(TEXT("RegisterServer"));

	CreateSession();
}

void AGDGameSession::RegisterPlayer(APlayerController* NewPlayer, const FUniqueNetIdRepl& UniqueId, bool bWasFromInvite)
{
	Super::RegisterPlayer(NewPlayer, UniqueId, bWasFromInvite);

	Logger.Log(TEXT("RegisterPlayer"));

}

bool AGDGameSession::CreateSession()
{
	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (!OnlineSub) 
		return false;

	// 获取会话接口
	IOnlineSessionPtr SessionPtr = OnlineSub->GetSessionInterface();
	if (!SessionPtr.IsValid())
		return false;
		
	// 配置会话参数
	FOnlineSessionSettings Settings;
	Settings.NumPublicConnections = 8;		// 公开的可用连接数
	Settings.bShouldAdvertise = true;		// 是否在在线服务上公开宣传。即客户端能不能主动查找到该会话。
	Settings.bAllowJoinInProgress = true;	// 是否允许游戏过程中加入。
	Settings.bIsDedicated = true;			// 是否为专用服务器。true：专用服务器，false：监听服务器。
	Settings.bIsLANMatch = true;			// 是否仅限局域网游戏，外网玩家不可见。
	Settings.bUsesPresence = false;			// 是否显示用户状态信息
	Settings.bAllowInvites = true;			// 是否允许邀请参加该会话
	Settings.bAllowJoinViaPresence = true;	// 是否允许通过玩家在场加入
	Settings.Set(SETTING_GAMEMODE, FString("StandMatch"), EOnlineDataAdvertisementType::ViaOnlineService);
	Settings.Set(SETTING_MAPNAME, GetWorld()->GetMapName(), EOnlineDataAdvertisementType::ViaOnlineService);
	Settings.Set(SETTING_MATCHING_TIMEOUT, 150.0f, EOnlineDataAdvertisementType::ViaOnlineService);

	// 监听会话创建是否完成
	OnCreateSessionCompleteDelegateHandle = SessionPtr->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &AGDGameSession::OnCreateSessionComplete));

	// 创建会话
	bool Result = SessionPtr->CreateSession(0, FName("NAME_GameSession2"), Settings);
	return Result;
}

void AGDGameSession::OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful)
{
	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (!OnlineSub) 
		return;

	// 获取会话接口
	IOnlineSessionPtr SessionPtr = OnlineSub->GetSessionInterface();
	if (!SessionPtr.IsValid())
		return;
	
	EOnlineSessionState::Type GameSessionState = SessionPtr->GetSessionState(InSessionName);
	SessionPtr->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
}
