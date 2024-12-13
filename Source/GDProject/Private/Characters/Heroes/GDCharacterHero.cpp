// Copyright 2020 Dan Kestranek.


#include "Characters/Heroes/GDCharacterHero.h"
#include "Player/GDPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayEffectTypes.h"
#include "InputMappingContext.h"
#include "AbilitySystemComponent.h"
#include "GDGameplayTags.h"
#include "Game/GDGameModeBase.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "UI/GDHUD.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/GDGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "Equipment/EquipmentManagerComponent.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Inventory/InventoryManagerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Niagara\Public\NiagaraComponent.h"

AGDCharacterHero::AGDCharacterHero()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->bDoCollisionTest = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	LevelUpNiagraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CharacterClass = ECharacterClass::Elementalist;
	
	// EffectAttachComponent = CreateDefaultSubobject<USceneComponent>("EffectAttachComponent");
	// EffectAttachComponent->SetupAttachment(RootComponent);
	
	HaloOfProtectionNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("HaloOfProtectionNiagaraComponent");
	HaloOfProtectionNiagaraComponent->SetupAttachment(RootComponent);
	LifeSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("LifeSiphonNiagaraComponent");
	LifeSiphonNiagaraComponent->SetupAttachment(RootComponent);
	ManaSiphonNiagaraComponent = CreateDefaultSubobject<UPassiveNiagaraComponent>("ManaSiphonNiagaraComponent");
	ManaSiphonNiagaraComponent->SetupAttachment(RootComponent);

	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryComponent"));
	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentInventoryComponent"));
}

void AGDCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 在服务端初始化
	InitAbilityActorInfo();
	LoadProgress();

	if (AGDGameModeBase* AuraGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuraGameMode->LoadWorldState(GetWorld());
	}
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

void AGDCharacterHero::LoadProgress()
{
	AGDGameModeBase* GameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(this));
	UGDAbilitySystemComponent* GDASC = Cast<UGDAbilitySystemComponent>(ASC);
	if (GameMode && GDASC)
	{
		ULoadScreenSaveGame* SaveData = GameMode->RetrieveInGameSaveData();
		if (!SaveData) return;

		// 第一次加载就使用默认属性
		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddSetupAbilities();
			AddSetupPassiveAbilities();
			GDASC->InitAbilityInfos();
		}
		// 后续加载需要用存档的数据
		else
		{
			// 初始被动能力不在技能树中，没有AbilityTag，不会存到存档里。
			AddSetupAbilities();
			AddSetupPassiveAbilities();
			GDASC->InitAbilityInfosFromSaveData(SaveData);
			
			if (AGDPlayerState* PS = Cast<AGDPlayerState>(GetPlayerState()))
			{
				PS->SetLevel(SaveData->PlayerLevel);
				PS->SetXP(SaveData->XP);
				PS->SetAttributePoints(SaveData->AttributePoints);
				PS->SetSpellPoints(SaveData->SpellPoints);
			}

			UGDAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, ASC, SaveData);
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
	OnAbilitySystemComponentAvailable();
	EquipmentManagerComponent->SetOwnerAbilitySystemComponent(ASC);
	
	// 初始化HUD
	if (APlayerController* PC = Cast<APlayerController>(GetController())) {
		if (AGDHUD* HUD = Cast<AGDHUD>(PC->GetHUD())) {
			HUD->InitOverlay(PC, PS, ASC, AS);
		}
	}
}

int32 AGDCharacterHero::GetPlayerLevel_Implementation() const
{
	return GetPlayerState<AGDPlayerState>()->GetPlayerLevel();
}

int32 AGDCharacterHero::GetXP_Implementation()
{
	return GetPlayerState<AGDPlayerState>()->GetXP();
}

void AGDCharacterHero::AddToXP_Implementation(int32 InXP)
{
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(GetPlayerState());
	GDPS->AddToXP(InXP);
}

int32 AGDCharacterHero::FindLevelForXP_Implementation(int32 XP)
{
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(GetPlayerState());
	
	if (GDPS->LevelUpInfo)
	{
		return GDPS->LevelUpInfo->FindLevelForXP(XP);
	}
	
	return 1;
}

int32 AGDCharacterHero::GetAttributePointsReward_Implementation(const int32 InLevel) const
{
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(GetPlayerState());
	
	if (GDPS->LevelUpInfo && GDPS->LevelUpInfo->LevelUpInfos.Num() > InLevel)
	{
		return GDPS->LevelUpInfo->LevelUpInfos[InLevel].AttributePointAward;
	}
	
	return 0;
}

int32 AGDCharacterHero::GetSpellPointsReward_Implementation(const int32 InLevel) const
{
	AGDPlayerState* GDPS = CastChecked<AGDPlayerState>(GetPlayerState());
	
	if (GDPS->LevelUpInfo && GDPS->LevelUpInfo->LevelUpInfos.Num() > InLevel)
	{
		return GDPS->LevelUpInfo->LevelUpInfos[InLevel].SpellPointAward;
	}
		
	return 0;
}

int32 AGDCharacterHero::GetAttributePoints_Implementation()
{
	return CastChecked<AGDPlayerState>(GetPlayerState())->GetAttributePoints();
}

int32 AGDCharacterHero::GetSpellPoints_Implementation()
{
	return CastChecked<AGDPlayerState>(GetPlayerState())->GetSpellPoints();
}

void AGDCharacterHero::AddToPlayerLevel_Implementation(int32 InLevel)
{
	AGDPlayerState* PS = CastChecked<AGDPlayerState>(GetPlayerState());
	PS->AddToLevel(InLevel);
	// 更新技能树
	if (UGDAbilitySystemComponent* GDASC = Cast<UGDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		GDASC->UpdateAbilityStatus(PS->GetPlayerLevel());
	}
}

void AGDCharacterHero::AddToAttributePoints_Implementation(int32 InPoints)
{
	CastChecked<AGDPlayerState>(GetPlayerState())->AddToAttributePoints(InPoints);
}

void AGDCharacterHero::AddToSpellPoints_Implementation(int32 InPoints)
{
	CastChecked<AGDPlayerState>(GetPlayerState())->AddToSpellPoints(InPoints);
}

void AGDCharacterHero::LevelUp_Implementation()
{
	Multicast_LevelUpParticles();
}

void AGDCharacterHero::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AGDGameModeBase* GameModeBase = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameModeBase)
	{
		ULoadScreenSaveGame* SaveData = GameModeBase->RetrieveInGameSaveData();
		AGDPlayerState* PS = Cast<AGDPlayerState>(GetPlayerState());
		if (SaveData && PS)
		{
			SaveData->PlayerStartTag = CheckpointTag;
			SaveData->PlayerLevel = PS->GetPlayerLevel();
			SaveData->XP = PS->GetXP();
			SaveData->SpellPoints = PS->GetSpellPoints();
			SaveData->AttributePoints = PS->GetAttributePoints();
			
			SaveData->Strength = UGDAttributeSet::GetStrengthAttribute().GetNumericValue(AS);
			SaveData->Intelligence = UGDAttributeSet::GetIntelligenceAttribute().GetNumericValue(AS);
			SaveData->Resilience = UGDAttributeSet::GetResilienceAttribute().GetNumericValue(AS);
			SaveData->Vigor = UGDAttributeSet::GetVigorAttribute().GetNumericValue(AS);
			SaveData->bFirstTimeLoadIn = false;

			UAbilityInfo* AbilityInfos = UGDAbilitySystemLibrary::GetAbilityInfo(this);
			UGDAbilitySystemComponent* GDASC = Cast<UGDAbilitySystemComponent>(ASC);
			SaveData->SavedAbilities.Empty();

			for (const auto& AbilityInfo : AbilityInfos->AbilityInfos) {
				auto AbilitySpec = GDASC->GetSpecFromAbilityTag(AbilityInfo.AbilityTag);
				FSavedAbility SavedAbility;

				SavedAbility.AbilityLevel = AbilitySpec ? AbilitySpec->Level : 1;
				
				SavedAbility.GameplayAbility = AbilityInfo.Ability;
				SavedAbility.AbilitySlot = AbilityInfo.InputTag;
				SavedAbility.AbilityStatus = AbilityInfo.StatusTag;
				SavedAbility.AbilityTag = AbilityInfo.AbilityTag;
				SavedAbility.AbilityType = AbilityInfo.AbilityType;
				SaveData->SavedAbilities.Add(SavedAbility);
			}
			
			GameModeBase->SaveInGameProgressData(SaveData);
		}
	}
}

UInventoryManagerComponent* AGDCharacterHero::GetInventoryManagerComponent() const {
    return InventoryManagerComponent;
}

UEquipmentManagerComponent* AGDCharacterHero::GetEquipmentManagerComponent() const {
    return EquipmentManagerComponent;
}

void AGDCharacterHero::OnRep_IsCanAttack()
{
	if (UGDAbilitySystemComponent* GDASC = Cast<UGDAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(FGDGameplayTags::Get().Player_Block_CursorTrace);
		BlockedTags.AddTag(FGDGameplayTags::Get().Player_Block_InputHeld);
		BlockedTags.AddTag(FGDGameplayTags::Get().Player_Block_InputPressed);
		BlockedTags.AddTag(FGDGameplayTags::Get().Player_Block_InputReleased);

		if (!bCanMove)
		{
			GDASC->AddLooseGameplayTags(BlockedTags);
		}
		else
		{
			GDASC->RemoveLooseGameplayTags(BlockedTags);
		}
	}
}

void AGDCharacterHero::Multicast_LevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagraComponent))
	{
		const FVector CameraLocation= CameraComponent->GetComponentLocation();
		const FVector NiagaraLocation= LevelUpNiagraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraLocation).Rotation();
		LevelUpNiagraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagraComponent->Activate(true);
	}
}