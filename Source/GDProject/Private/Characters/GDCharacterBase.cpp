// Copyright 2020 Dan Kestranek.


#include "Characters/GDCharacterBase.h"
#include "AbilitySystemComponent.h"

#include "GameplayEffect.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GDProject/GDProject.h"
#include "Kismet/GameplayStatics.h"

AGDCharacterBase::AGDCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	const FGDGameplayTags& GameplayTags = FGDGameplayTags::Get();
	
	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = GameplayTags.Debuff_Burn;
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponComponent->SetupAttachment(GetMesh(), FName(TEXT("WeaponHandSocket")));
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
}

void AGDCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);

		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		WeaponComponent->SetMaterial(0, DynamicMatInst);

		StartWeaponDissolveTimeline(DynamicMatInst);
	}
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
	CastChecked<UGDAbilitySystemComponent>(GetAbilitySystemComponent())->AddCharacterPassiveAbilities(SetupPassiveAbilities);
}

FVector AGDCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag)
{
	FGDGameplayTags& GameplayTags = FGDGameplayTags::Get();
	if (CombatSocketTag == GameplayTags.CombatSocket_Weapon)
	{
		return WeaponComponent->GetSocketLocation(WeaponTipSocketName);
	}
	if (CombatSocketTag == GameplayTags.CombatSocket_Tail)
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	if (CombatSocketTag == GameplayTags.CombatSocket_LeftHand)
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (CombatSocketTag == GameplayTags.CombatSocket_RightHand)
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	return FVector();
}

UAnimMontage* AGDCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AGDCharacterBase::Die()
{
	// 分离武器，让武器掉到地上
	WeaponComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));;
	MulticastHandleDeath();
}

bool AGDCharacterBase::IsDead_Implementation() const
{
	return bIsDead;
}

AActor* AGDCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AGDCharacterBase::GetTaggedMontages_Implementation()
{
	return AttackMongages;
}

UNiagaraSystem* AGDCharacterBase::GetBloodEffect_Implementation() const
{
	return BloodEffect;
}

int32 AGDCharacterBase::GetMinionCount_Implementation() const
{
	return MinionCount;
}

ECharacterClass AGDCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered AGDCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

void AGDCharacterBase::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeadSound, GetActorLocation());
	
	WeaponComponent->SetSimulatePhysics(true);
	WeaponComponent->SetEnableGravity(true);
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	// 先打开模拟物理，再设置碰撞检测包含物理，就能进入布娃娃状态，不会使用角色动画
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bIsDead = true;
}

void AGDCharacterBase::InitializeDefaultAttributes() const
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
