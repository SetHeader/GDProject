// Copyright 2020 Dan Kestranek.


#include "Characters/Minions/GDCharacterMinion.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/GDAttributeSetBase.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/GDUserWidget.h"

AGDCharacterMinion::AGDCharacterMinion()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	ASC = CreateDefaultSubobject<UGDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AS = CreateDefaultSubobject<UGDAttributeSetBase>(TEXT("AttributeSetBase"));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(GetRootComponent());
}

void AGDCharacterMinion::BeginPlay()
{
	Super::BeginPlay();
	
	ASC->InitAbilityActorInfo(this, this);
	CastChecked<UGDAbilitySystemComponent>(ASC)->OnAbilityActorInfoSet();
	InitializeAttributes();

	if (UGDUserWidget* AuraUserWidget = Cast<UGDUserWidget>(WidgetComponent->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
		
		BindCallbacksToDependencies();
		BroadcastInitialValues();
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

void AGDCharacterMinion::InitializeAttributes() const
{
	UGDAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, ASC);
}
