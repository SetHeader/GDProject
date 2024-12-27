// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/GDEntryGameSession.h"
#include "UI/Base/WidgetController.h"
#include "GDMainMenuWidgetController.generated.h"

class USaveGameSubsystem;
class USimpleUserWidget;
class FUniqueNetId;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionFoundSignature, const TArray<FUISessionInfo>&, SessionInfos);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSessionInfoChangedSignature);

UENUM(BlueprintType)
enum class EMainMenuSelection : uint8 {
	RUN_IN_LOCAL,
	CREATE_ROOM,
	FIND_ROOM
};

UCLASS(BlueprintType)
class GDPROJECT_API UGDMainMenuWidgetController : public UWidgetController {
	GENERATED_BODY()

public:
	virtual void OnInitialized() override;
	virtual void BeginDestroy() override;
	// 路由到新组件
	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	void RouteTo(TSubclassOf<UUserWidget> UserWidgetClass);
	// 回退一步
	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	void RouteBack();
	// 回退到第一个组件
	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	void RouteBackToFirst();
	// 路由到新组件，替换掉当前组件
	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	void RerouteTo(TSubclassOf<UUserWidget> UserWidgetClass);
	
	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	void SetCurrentSelection(EMainMenuSelection NewSelection) { CurrentSelection = NewSelection; }
	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	EMainMenuSelection GetCurrentSelection() const { return CurrentSelection; }

	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	void PlayGameInLocal();

	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	void FindSession();
	// 加入会话，会加入到当前选择的会话中
	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	bool JoinSession();
	
	UFUNCTION(BlueprintCallable, Category = "UGDMainMenuWidgetController")
	void SetSelectedSessionInfo(FUISessionInfo SessionInfo);

public:
	UPROPERTY(BlueprintAssignable, Category = "UGDMainMenuWidgetController")
	FOnSessionFoundSignature OnSessionFoundDelegate;

	UPROPERTY(BlueprintAssignable, Category = "UGDMainMenuWidgetController")
	FOnSessionInfoChangedSignature OnSessionInfoChangedDelegate;

	UPROPERTY(BlueprintReadOnly, Category = "UGDMainMenuWidgetController")
	FUISessionInfo SelectedSession;
private:
	UPROPERTY()
	TObjectPtr<USaveGameSubsystem> SaveGameSubsystem;
	
	EMainMenuSelection CurrentSelection;

	UPROPERTY()
	TArray<TObjectPtr<UUserWidget>> WidgetStack;

	FDelegateHandle FindSessionHandle;
};
