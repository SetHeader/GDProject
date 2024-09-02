// Copyright 2020 Dan Kestranek.


#include "Characters/GDCharacterBase.h"
#include "AbilitySystemComponent.h"

#include "GameplayEffect.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "GDProject/GDProject.h"

AGDCharacterBase::AGDCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponComponent->SetupAttachment(GetMesh(), FName(TEXT("WeaponHandSocket")));
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
}

void AGDCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (GetLocalRole() != ROLE_Authority) {
		return;
	}
	
	if (!ASC) {
		return;
	}
}

void AGDCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority())
	{
		return;
	}

	CastChecked<UGDAbilitySystemComponent>(GetAbilitySystemComponent())->AddCharacterAbilities(SetupAbilities);
}

void AGDCharacterBase::InitializeAttributes() const
{
	check(ASC);
	check(DefaultPrimaryAttributes);
	check(DefaultSecondaryAttributes);
	check(DefaultVitalAttributes);

	FGameplayEffectContextHandle AEContextHandle = ASC->MakeEffectContext();
	AEContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle AESpecHandle = ASC->MakeOutgoingSpec(DefaultPrimaryAttributes, 1, AEContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*AESpecHandle.Data);

	FGameplayEffectSpecHandle SecondaryAESpecHandle = ASC->MakeOutgoingSpec(DefaultSecondaryAttributes, 1, AEContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAESpecHandle.Data);

	FGameplayEffectSpecHandle VitalAESpecHandle = ASC->MakeOutgoingSpec(DefaultVitalAttributes, 1, AEContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAESpecHandle.Data);
}
