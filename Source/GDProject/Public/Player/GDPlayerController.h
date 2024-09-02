// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GDPlayerController.generated.h"

class USplineComponent;
struct FGameplayTag;
class UGDInputConfig;
class UGDAbilitySystemComponent;
class IEnemyInterface;

/**
 * 
 */
UCLASS()
class GDPROJECT_API AGDPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	TObjectPtr<UGDAbilitySystemComponent> ASC;
	
	UPROPERTY(EditDefaultsOnly, Category="GDPlayerController")
	TObjectPtr<UGDInputConfig> InputConfig;

	// 存储每帧的鼠标追踪结果
	FHitResult CursorHit;

	/*
	 * 下面的属性用来处理鼠标点击 自动行走
	 * 自动行走分为两个功能
	 * 1、鼠标一直按住时，角色就朝鼠标方向直线行走，不需要处理障碍物。
	 * 2、鼠标按下再释放时，角色朝鼠标位置进行导航行走，会避开障碍物。
	 */
	// 鼠标按下时，判断是否要进入自动行走逻辑，为true就行走，为false就触发能力
	bool bTriggerAutoRunning;
	// 鼠标左键 按下 到 释放 的时间间隔
	float FollowTime;
	// 触发导航行走的阈值
	UPROPERTY(EditDefaultsOnly, Category="GDPlayerController")
	float NavRunningTime = 0.5f;
	// 导航到目的地的误差距离
	UPROPERTY(EditDefaultsOnly, Category="GDPlayerController")
	float NavDestinationError = 100.f;
	// 是否正在自动行走，为true时，每帧都会移动角色
	bool bAutoRunning;
	// 自动行走的终点位置
	FVector Destination;
	// 用于把离散的点位置 转换成圆滑的 位置曲线
	TObjectPtr<USplineComponent> SplineComponent; 
	
public:
	AGDPlayerController();
	void BeginPlay() override;

	void PlayerTick(float DeltaTime) override;

	void OnPossess(APawn* aPawn) override;

	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);
	void OnAbilityInputHeld(FGameplayTag InputTag);

	inline UGDAbilitySystemComponent* GetASC();

protected:
	virtual void SetupInputComponent() override;

private:
	void CursorTrace();
	
	void AutoRun();
};