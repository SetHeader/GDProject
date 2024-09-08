// Copyright 2020 Dan Kestranek.


#include "Characters/Heroes/GDCharacterHero.h"
#include "Player/GDPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbility.h"
#include "../GDProject.h"
#include "InputMappingContext.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "UI/GDHUD.h"
#include "UI/WidgetController/GDWidgetController.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"

void AGDCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 在服务端初始化
	InitAbilityActorInfo();
	InitializeAttributes();

	AddCharacterAbilities();
}

void AGDCharacterHero::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 在客户端初始化
	InitAbilityActorInfo();
}

void AGDCharacterHero::AddIMC(UInputMappingContext* IMC, int32 Priority)
{
	APlayerController* PlayController = Cast<APlayerController>(GetController());
	if (PlayController) {
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayController->GetLocalPlayer());

		if (EnhancedInputSubsystem) {
			EnhancedInputSubsystem->AddMappingContext(IMC, Priority);
		}
	}
}

void AGDCharacterHero::RemoveIMC(UInputMappingContext* IMC)
{
	APlayerController* PlayController = Cast<APlayerController>(GetController());
	if (PlayController) {
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayController->GetLocalPlayer());

		if (EnhancedInputSubsystem) {
			EnhancedInputSubsystem->RemoveMappingContext(IMC);
		}
	}
}

void AGDCharacterHero::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// 绑定初始输入
	for (UInputMappingContext* IMC : InitialIMCArray) {
		AddIMC(IMC);
	}
}

void AGDCharacterHero::InitAbilityActorInfo()
{
	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	check(PS);

	// 需要初始化ASC，保证avatar actor是pawn（角色是ROLE_AutonomousProxy）这样才能在客户端调激活技能。
	// 默认 avatar actor是pawn 会是 GDPlayerState（角色是ROLE_SimulatedProxy），不能在客户端调激活技能
	PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	ASC = PS->GetAbilitySystemComponent();
	AS = PS->GetGDASBase();
	CastChecked<UGDAbilitySystemComponent>(ASC)->OnAbilityActorInfoSet();

	// 初始化HUD
	if (APlayerController* PC = Cast<APlayerController>(GetController())) {
		if (AGDHUD* HUD = Cast<AGDHUD>(PC->GetHUD())) {
			HUD->InitOverlay(PC, PS, ASC, AS);
		}
	}
}

int32 AGDCharacterHero::GetPlayerLevel() const
{
	return GetPlayerState<AGDPlayerState>()->GetPlayerLevel();
}
