// Copyright 2020 Dan Kestranek.


#include "Player/GDPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GDGameplayTags.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/GDInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"

AGDPlayerController::AGDPlayerController()
{
	bReplicates = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void AGDPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 显示鼠标
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	// 不要把鼠标锁定在游戏视口中，即鼠标可以移出游戏
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// 不要在抓取的时候隐藏鼠标
	InputMode.SetHideCursorDuringCapture(false);

	SetInputMode(InputMode);
}

void AGDPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
	AutoRun();
}

void AGDPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

inline UGDAbilitySystemComponent* AGDPlayerController::GetASC()
{
	if (!ASC)
	{
		ASC = Cast<UGDAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return ASC;
}

void AGDPlayerController::Client_ShowDamageNumber_Implementation(float Damage, ACharacter* Target, const bool bBlockedHit, const bool bCriticalHit)
{
	// IsValid不只是检查非空，还会检查指针是否准备销毁
	if (IsValid(Target) && DamageTextComponentClass)
	{
		// 不在构造函数中不能调 CreateDefaultSubobject，需要动态NewObject出来。
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(Target, DamageTextComponentClass);
		// 动态创建的组件需要调用 RegisterComponent 方法，处理一些内部逻辑
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->SetDamageText(Damage, bBlockedHit, bCriticalHit);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AGDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputConfig);
	UGDInputComponent* IC = CastChecked<UGDInputComponent>(InputComponent);
	IC->SetAbilityInputConfig(InputConfig.Get(), this, &ThisClass::OnAbilityInputPressed, &ThisClass::OnAbilityInputReleased, &ThisClass::OnAbilityInputHeld);
}

void AGDPlayerController::OnAbilityInputPressed(FGameplayTag InputTag)
{
	// 判断是否要触发行走
	if (FGDGameplayTags::Get().InputTag_LMB.MatchesTagExact(InputTag))
	{
		if (!ThisActor && CursorHit.bBlockingHit)
		{
			bAutoRunning = false;
			bTriggerAutoRunning = true;
			Destination = CursorHit.ImpactPoint;
			//DrawDebugSphere(GetWorld(), CursorHit.ImpactPoint, 8, 8, FColor::Red, false, 10.f);
			return;
		}
	}

	// 触发能力
	if (UGDAbilitySystemComponent* AbilitySystemComponent = GetASC())
	{
		AbilitySystemComponent->OnAbilityInputPressed(InputTag);
	}
}

void AGDPlayerController::OnAbilityInputReleased(FGameplayTag InputTag)
{
	// 配置导航行走
	if (FGDGameplayTags::Get().InputTag_LMB.MatchesTagExact(InputTag))
	{
		if (!bTriggerAutoRunning)
		{
			return;
		}
		
		if (GetPawn() && FollowTime <= NavRunningTime)
		{
			// 获取导航路径，并把路径点添加到Spline组件中
			UNavigationPath* NavigationPath = UNavigationSystemV1::GetNavigationSystem(GetWorld())->FindPathToLocationSynchronously(GetWorld(), GetPawn()->GetActorLocation(), Destination);
			if (NavigationPath)
			{
				SplineComponent->ClearSplinePoints();
				TArray<FVector>& PathPoints = NavigationPath->PathPoints;
				for (const FVector& PathPoint : PathPoints)
				{
					SplineComponent->AddSplineWorldPoint(PathPoint);
					//DrawDebugSphere(GetWorld(), PathPoint, 8, 8, FColor::Green, false, 10.f);
				}
				if (!PathPoints.IsEmpty())
				{
					// 更新目的地为导航路径的终点
					Destination = PathPoints[PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}

		// 重置自动行走相关变量
		bTriggerAutoRunning = false;
		FollowTime = 0.f;
	}

	// 触发能力
	if (UGDAbilitySystemComponent* AbilitySystemComponent = GetASC())
	{
		AbilitySystemComponent->OnAbilityInputReleased(InputTag);
	}
}

void AGDPlayerController::OnAbilityInputHeld(FGameplayTag InputTag)
{
	// 直线行走
	if (FGDGameplayTags::Get().InputTag_LMB.MatchesTagExact(InputTag))
	{
		if  (bTriggerAutoRunning && GetPawn())
		{
			FollowTime += GetWorld()->GetDeltaSeconds();
			if (CursorHit.bBlockingHit)
			{
				Destination = CursorHit.ImpactPoint;
				FVector Direction = (Destination  - GetPawn()->GetActorLocation()).GetSafeNormal(); 
				GetPawn()->AddMovementInput(Direction);
			}
			return;
		}
	}
	// 触发能力
	if (UGDAbilitySystemComponent* AbilitySystemComponent = GetASC())
	{
		AbilitySystemComponent->OnAbilityInputHeld(InputTag);
	}
}

void AGDPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) {
		return;
	}

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	// 1、 LastActor、ThisActor 都是空，不处理
	// 2、 LastActor 有效，ThisActor 无效，则UnHighLight LastActor
	// 3、 LastActor 无效，ThisActor 有效，则HighLight ThisActor
	// 4、 LastActor、ThisActor 都不为空，且不相等，则 UnHighLight LastActor 并且 HighLight ThisActor
	// 5、 LastActor、ThisActor 都不为空，且相等，不处理

	if (ThisActor != LastActor)
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
}

void AGDPlayerController::AutoRun()
{
	if (!bAutoRunning)
	{
		return;
	}

	if (!GetPawn())
	{
		return;
	}

	// 小于误差就结束
	if ((Destination - GetPawn()->GetActorLocation()).Length() < NavDestinationError)
	{
		bAutoRunning = false;
		return;
	}

	FVector ClosestLocation = SplineComponent->FindLocationClosestToWorldLocation(GetPawn()->GetActorLocation(), ESplineCoordinateSpace::World);
	FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(ClosestLocation, ESplineCoordinateSpace::World);
	GetPawn()->AddMovementInput(Direction);
}
