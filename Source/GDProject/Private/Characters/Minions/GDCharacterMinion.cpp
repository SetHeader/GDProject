// Copyright 2020 Dan Kestranek.


#include "Characters/Minions/GDCharacterMinion.h"
#include "AbilitySystemComponent.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/AttributeSets/GDAttributeSet.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AI/GDAIController.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "UI/Widget/GDUserWidget.h"

AGDCharacterMinion::AGDCharacterMinion()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	// 这样播放蒙太奇时，服务端也能查询到骨骼位置信息
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickMontagesAndRefreshBonesWhenPlayingMontages;
	
	ASC = CreateDefaultSubobject<UGDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AS = CreateDefaultSubobject<UGDAttributeSet>(TEXT("AttributeSetBase"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void AGDCharacterMinion::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	ASC->InitAbilityActorInfo(this, this);
	CastChecked<UGDAbilitySystemComponent>(ASC)->OnAbilityActorInfoSet();
	if (HasAuthority())
	{
		InitializeAttributes();
		UGDAbilitySystemLibrary::GiveStartupAbilities(this, ASC, CharacterClass);
	}
	
	if (UGDUserWidget* AuraUserWidget = Cast<UGDUserWidget>(WidgetComponent->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
		
		BindCallbacksToDependencies();
		BroadcastInitialValues();
	}

	ASC->RegisterGameplayTagEvent(FGDGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &AGDCharacterMinion::HitReactTagChanged);
}

void AGDCharacterMinion::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	
	GDAIController = Cast<AGDAIController>(NewController);
	if (!GDAIController)
	{
		return;
	}
	GDAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	GDAIController->RunBehaviorTree(BehaviorTree);

	// 设置黑板键
	GDAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	
	if (CharacterClass == ECharacterClass::Warrior)
	{
		GDAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), false);
	} else
	{
		GDAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), true);
	}
}

void AGDCharacterMinion::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(250);
	WeaponComponent->SetRenderCustomDepth(true);
	WeaponComponent->SetCustomDepthStencilValue(250);
}


void AGDCharacterMinion::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
	WeaponComponent->SetRenderCustomDepth(false);
	WeaponComponent->SetCustomDepthStencilValue(0);
}

void AGDCharacterMinion::BroadcastInitialValues() const
{
	check(AS);
	OnHealthChanged.Broadcast(AS->GetHealth());
	OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
}

void AGDCharacterMinion::BindCallbacksToDependencies() const
{
	check(ASC);
	check(AS);
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangedData)
	{
		OnHealthChanged.Broadcast(ChangedData.NewValue);
	});
	ASC->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& ChangedData)
	{
		OnMaxHealthChanged.Broadcast(ChangedData.NewValue);
	});
}

void AGDCharacterMinion::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (GDAIController)
	{
		GDAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AGDCharacterMinion::Die()
{
	SetLifeSpan(LifeSpan);
	if (GDAIController)
	{
		GDAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	Super::Die();
}

void AGDCharacterMinion::InitializeAttributes() const
{
	UGDAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, ASC);
}

AActor* AGDCharacterMinion::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void AGDCharacterMinion::SetCombatTarget_Implementation(AActor* Target)
{
	CombatTarget = Target;
}
