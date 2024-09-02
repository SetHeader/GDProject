// Copyright 2020 Dan Kestranek.


#include "Characters/Minions/GDCharacterMinion.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSets/GDAttributeSetBase.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"

AGDCharacterMinion::AGDCharacterMinion()
{
	ASC = CreateDefaultSubobject<UGDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AS = CreateDefaultSubobject<UGDAttributeSetBase>(TEXT("AttributeSetBase"));
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

void AGDCharacterMinion::BeginPlay()
{
	Super::BeginPlay();
	
	ASC->InitAbilityActorInfo(this, this);
	CastChecked<UGDAbilitySystemComponent>(ASC)->OnAbilityActorInfoSet();
	InitializeAttributes();
}
