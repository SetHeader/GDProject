// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDAbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Game/GDGameModeBase.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/GDAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Characters/Minions/GDCharacterMinion.h"
#include "Game/LoadScreenSaveGame.h"
#include "GDProject/GDLog.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/GDHUD.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "GameplayAbilities\Public\GameplayEffectTypes.h"

UGDOverlayWidgetController* UGDAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AGDHUD* GDHUD = Cast<AGDHUD>(PC->GetHUD()))
		{
			return Cast<UGDOverlayWidgetController>(GDHUD->GetOverlayWidgetController());
		}
	}
	return nullptr;
}

UGDAttributeMenuWidgetController* UGDAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AGDHUD* GDHUD = Cast<AGDHUD>(PC->GetHUD()))
		{
			return Cast<UGDAttributeMenuWidgetController>(GDHUD->GetAttributeMenuWidgetController());
		}
	}
	return nullptr;
}

USpellMenuWidgetController* UGDAbilitySystemLibrary::GetSpellMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AGDHUD* GDHUD = Cast<AGDHUD>(PC->GetHUD()))
		{
			return Cast<USpellMenuWidgetController>(GDHUD->GetSpellMenuWidgetController());
		}
	}
	return nullptr;
}

void UGDAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	checkf(WorldContextObject, TEXT("WorldContextObject is null"));
	checkf(ASC, TEXT("ASC is null"));
	
	AGDGameModeBase* GDGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(GDGameMode))
	{
		return;
	}
	
	UCharacterClassInfo* ClassInfo = GDGameMode->CharacterClassInfo;
	checkf(ClassInfo, TEXT("ClassInfo is null"));
	checkf(ClassInfo->SecondaryAttributes, TEXT("ClassInfo->SecondaryAttributes is null"));
	checkf(ClassInfo->VitalAttributes, TEXT("ClassInfo->VitalAttributes is null"));
	
	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());
	
	FGameplayEffectSpecHandle PrimaryEffectSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, EffectContextHandle);
	FGameplayEffectSpecHandle SecondaryEffectSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, EffectContextHandle);
	FGameplayEffectSpecHandle VitalEffectSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, EffectContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data);
	
}

void UGDAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(const UObject* WorldContextObject,
	UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return;

	const FGDGameplayTags& GameplayTags = FGDGameplayTags::Get();

	const AActor* SourceAvatarActor = ASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());

	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Primary_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Attribute_Primary_Vigor, SaveGame->Vigor);
	
	FGameplayEffectSpecHandle SecondaryEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes_Infinite, 1.f, EffectContextHandle);
	FGameplayEffectSpecHandle VitalEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, 1.f, EffectContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data);
}

void UGDAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	checkf(WorldContextObject, TEXT("WorldContextObject is null"));
	checkf(ASC, TEXT("ASC is null"));
	
	AGDGameModeBase* GDGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(GDGameMode))
	{
		return;
	}

	UCharacterClassInfo* ClassInfo = GDGameMode->CharacterClassInfo;
	if (!ClassInfo)
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility> CommonAbility : ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
	if (!IsValid(ClassInfo))
	{
		return;
	}
	for (TSubclassOf<UGameplayAbility> CommonAbility : ClassDefaultInfo.CommonAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			const int32 Level = ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor());
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CommonAbility, Level);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

const UCurveTable* UGDAbilitySystemLibrary::GetDamageCoefficientsCurveTable(const UObject* WorldContextObject)
{
	AGDGameModeBase* GDGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(GDGameMode))
	{
		return nullptr;
	}

	UCharacterClassInfo* ClassInfo = GDGameMode->CharacterClassInfo;
	checkf(ClassInfo, TEXT("ClassInfo is null"));

	return ClassInfo->DamageCalculationCoefficients;
}

bool UGDAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGDGameplayEffectContext* Context = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get());
	if (Context)
	{
		return Context->IsBlockedHit();
	}
	
	UE_LOG(LogTemp, Error, TEXT("UGDAbilitySystemLibrary\t Can't Cast To FGDGameplayEffectContext."));
	return false;
}

bool UGDAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const FGDGameplayEffectContext* Context = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get());
	if (Context)
	{
		return Context->IsCriticalHit();
	}
	
	UE_LOG(LogTemp, Error, TEXT("UGDAbilitySystemLibrary\t Can't Cast To FGDGameplayEffectContext."));
	return false;
}

void UGDAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bValue)
{
	FGDGameplayEffectContext* Context = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get());
	if (Context)
	{
		Context->SetIsBlockedHit(bValue);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("UGDAbilitySystemLibrary\t Can't Cast To FGDGameplayEffectContext."));
	}
}

void UGDAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bValue)
{
	FGDGameplayEffectContext* Context = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get());
	if (Context)
	{
		Context->SetIsCriticalHit(bValue);
	} else
	{
		UE_LOG(LogTemp, Error, TEXT("UGDAbilitySystemLibrary\t Can't Cast To FGDGameplayEffectContext."));
	}
}

void UGDAbilitySystemLibrary::GetLivePlayersWithInRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	
	UWorld* World = WorldContextObject->GetWorld();

	if (World)
	{
		FCollisionObjectQueryParams ObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(ActorsToIgnore);
		
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(Radius), QueryParams);

		for (const FOverlapResult& Overlap : Overlaps)
		{
			AActor* OverlapActor = Overlap.GetActor();
			
			// 注意这里是使用UCombatInterface，不是ICombatInterface
			const bool ImplementsCombatInterface = OverlapActor->Implements<UCombatInterface>();
			
			if (ImplementsCombatInterface)
			{
				if (!ICombatInterface::Execute_IsDead(OverlapActor)) {
					OutOverlappingActors.Add(ICombatInterface::Execute_GetAvatar(OverlapActor));
				}
			}
		}
	}
}

void UGDAbilitySystemLibrary::GetClosestTargets(int32 MaxTarget, const TArray<AActor*>& Actors,
	TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (MaxTarget >= Actors.Num())
	{
		for (AActor* Actor : Actors)
		{
			OutClosestTargets.Add(Actor);
		}
		return;
	}

	TArray<AActor*> TempActors(Actors);
	
	for (int i = 0; i < MaxTarget; ++i)
	{
		int32 MinActorIdx = 0;
		float MinDist = FLT_MAX;
		
		for (int j = 0; j < TempActors.Num(); ++j)
		{
			AActor* Actor = TempActors[j];
			float Dist = FVector::Distance(Origin, Actor->GetActorLocation());
			if (Dist < MinDist)
			{
				MinDist = Dist;
				MinActorIdx = j;
			}
		}
		
		OutClosestTargets.Add(TempActors[MinActorIdx]);
		TempActors.RemoveAt(MinActorIdx);
	}
}

FGameplayTagContainer UGDAbilitySystemLibrary::FindInputTagFromAbilitySpec(const FGameplayAbilitySpec* AbilitySpec)
{
	FGameplayTag InputTagPrefix = FGameplayTag::RequestGameplayTag("InputTag");
	return  AbilitySpec->DynamicAbilityTags.Filter(FGameplayTagContainer(InputTagPrefix));
}

void UGDAbilitySystemLibrary::ResetInputTagFromAbilitySpec(FGameplayAbilitySpec* AbilitySpec,
	const FGameplayTag& NewInputTag)
{
	AbilitySpec->DynamicAbilityTags.RemoveTags(FindInputTagFromAbilitySpec(AbilitySpec));
	if (NewInputTag.IsValid())
	{
		AbilitySpec->DynamicAbilityTags.AddTag(NewInputTag);
	}
}

FGameplayTagContainer UGDAbilitySystemLibrary::FindStatusTagFromAbilitySpec(const FGameplayAbilitySpec* AbilitySpec)
{
	FGameplayTag StatusTagPrefix = FGameplayTag::RequestGameplayTag("Abilities.Status");
	return  AbilitySpec->DynamicAbilityTags.Filter(FGameplayTagContainer(StatusTagPrefix));
}

void UGDAbilitySystemLibrary::ResetStatusTagFromAbilitySpec(FGameplayAbilitySpec* AbilitySpec,
                                                            const FGameplayTag& NewStatusTag)
{
	AbilitySpec->DynamicAbilityTags.RemoveTags(FindStatusTagFromAbilitySpec(AbilitySpec));
	if (NewStatusTag.IsValid())
	{
		AbilitySpec->DynamicAbilityTags.AddTag(NewStatusTag);
	}
}

bool UGDAbilitySystemLibrary::IsNotFriend(const AActor* FirstActor, const AActor* SecondActor)
{
	if (IsValid(FirstActor) && IsValid(SecondActor))
	{
		const bool bBothArePlayers = FirstActor->ActorHasTag("Player") && SecondActor->ActorHasTag("Player");
		const bool bBothAreEnemies = FirstActor->ActorHasTag("Enemy") && SecondActor->ActorHasTag("Enemy");
		return !(bBothArePlayers && bBothAreEnemies);
	}
	return true;
}

ECharacterClass UGDAbilitySystemLibrary::GetCharacterClass(AActor* Character)
{
	return ICombatInterface::Execute_GetCharacterClass(Character);
}

int32 UGDAbilitySystemLibrary::GetCharacterLevel(const AActor* Character)
{
	if (Character->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetPlayerLevel(Character);
	}
	
	UE_LOG(LogGD, Warning, TEXT("%hs\t Character Is Not Valid"), __FUNCTION__)
	return 1;
}

int32 UGDAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	AGDGameModeBase* GDGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(GDGameMode))
	{
		return 0;
	}

	UCharacterClassInfo* ClassInfo = GDGameMode->CharacterClassInfo;
	if (!ClassInfo)
	{
		return 0;
	}

	FCharacterClassDefaultInfo* ClassDefaultInfo = ClassInfo->CharacterClassInformation.Find(CharacterClass);
	if (ClassDefaultInfo && ClassDefaultInfo->XPReward.IsValid())
	{
		return ClassDefaultInfo->XPReward.GetValueAtLevel(CharacterLevel);
	}

	return 0;
}

UCharacterClassInfo* UGDAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AGDGameModeBase* GDGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(GDGameMode))
	{
		return nullptr;
	}

	return GDGameMode->CharacterClassInfo;
}

UAbilityInfo* UGDAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	AGDGameModeBase* GDGameMode = Cast<AGDGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(GDGameMode))
	{
		return nullptr;
	}

	return GDGameMode->GetAbilityInfo();
}

FGameplayEffectContextHandle UGDAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	if (!IsValid(DamageEffectParams.TargetAbilitySystemComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("UGDAbilitySystemLibrary::ApplyDamageEffect: TargetAbilitySystemComponent is invalid"))
		return FGameplayEffectContextHandle();
	}
	
	const FGDGameplayTags& GameplayTags = FGDGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	
	SetDeathImpulse(EffectContexthandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContexthandle, DamageEffectParams.KnockbackForce);
	
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
	
	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContexthandle;
}

bool UGDAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* GDEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return GDEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UGDAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* GDEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return GDEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float UGDAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* GDEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return GDEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float UGDAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* GDEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return GDEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag UGDAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* GDEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return *GDEffectContext->GetDamageType();
	}
	return FGameplayTag();
}

FVector UGDAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* AuraEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UGDAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* AuraEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

void UGDAbilitySystemLibrary::SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle,
                                                    bool bInSuccessfulDebuff)
{
	if (FGDGameplayEffectContext* GDEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return GDEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void UGDAbilitySystemLibrary::SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FGDGameplayEffectContext* GDEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return GDEffectContext->SetDebuffDamage(InDamage);
	}
}

void UGDAbilitySystemLibrary::SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FGDGameplayEffectContext* GDEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return GDEffectContext->SetDebuffDuration(InDuration);
	}
}

void UGDAbilitySystemLibrary::SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FGDGameplayEffectContext* GDEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return GDEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void UGDAbilitySystemLibrary::SetDamageType(FGameplayEffectContextHandle& EffectContextHandle,
	const FGameplayTag& InDamageType)
{
	if (FGDGameplayEffectContext* GDEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		return GDEffectContext->SetDamageType(DamageType);
	}
}

void UGDAbilitySystemLibrary::SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InImpulse)
{
	if (FGDGameplayEffectContext* AuraEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UGDAbilitySystemLibrary::SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle,
                                                const FVector& InForce)
{
	if (FGDGameplayEffectContext* AuraEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->SetKnockbackForce(InForce);
	}
}

bool UGDAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* AuraEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsRadialDamage();
	}
	return false;
}

float UGDAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* AuraEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.f;
}

float UGDAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* AuraEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.f;
}

FVector UGDAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FGDGameplayEffectContext* AuraEffectContext = static_cast<const FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UGDAbilitySystemLibrary::SetIsRadialDamage(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsRadialDamage)
{
	if (FGDGameplayEffectContext* AuraEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UGDAbilitySystemLibrary::SetRadialDamageInnerRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InInnerRadius)
{
	if (FGDGameplayEffectContext* AuraEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UGDAbilitySystemLibrary::SetRadialDamageOuterRadius(FGameplayEffectContextHandle& EffectContextHandle,
	float InOuterRadius)
{
	if (FGDGameplayEffectContext* AuraEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UGDAbilitySystemLibrary::SetRadialDamageOrigin(FGameplayEffectContextHandle& EffectContextHandle,
	const FVector& InOrigin)
{
	if (FGDGameplayEffectContext* AuraEffectContext = static_cast<FGDGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}