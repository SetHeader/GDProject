// Copyright 2020 Dan Kestranek.

#include "Player/GDPlayerState.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystem/AttributeSets/GDAttributeSet.h>
#include "GameplayEffectTypes.h"
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

AGDPlayerState::AGDPlayerState()
{
	ASC = CreateDefaultSubobject<UGDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AS = CreateDefaultSubobject<UGDAttributeSet>(TEXT("AttributeSetBase"));

	// 自定义网络复制频率
	NetUpdateFrequency = 100.f;
}

void AGDPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AGDPlayerState, Level, COND_None, REPNOTIFY_Always);
}

float AGDPlayerState::GetHealth() const
{
	if (AS) {
		return AS->GetHealth();
	}
	else {
		return 0.f;
	}
}

float AGDPlayerState::GetMaxHealth() const
{
	if (AS) {
		return AS->GetMaxHealth();
	}
	else {
		return 0.f;
	}
}

float AGDPlayerState::GetMana() const
{
	if (AS) {
		return AS->GetMana();
	}
	else {
		return 0.f;
	}
}

float AGDPlayerState::GetMaxMana() const
{
	if (AS) {
		return AS->GetMaxMana();
	}
	else {
		return 0.f;
	}
}

float AGDPlayerState::GetStamina() const
{
	if (AS) {
		return AS->GetStamina();
	}
	else {
		return 0.f;
	}
}

float AGDPlayerState::GetMaxStamina() const
{
	if (AS) {
		return AS->GetMaxStamina();
	}
	else {
		return 0.f;
	}
}

void AGDPlayerState::OnRep_Level(const int32 OldLevel)
{
	
}
