// Copyright 2020 Dan Kestranek.

#include "Player/GDPlayerState.h"
#include "AbilitySystemComponent.h"
#include <AbilitySystem/AttributeSets/GDAttributeSet.h>
#include "AbilitySystem/GDAbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GDProject/GDLog.h"
#include "Tools/FCheckTool.h"

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
	DOREPLIFETIME_CONDITION_NOTIFY(AGDPlayerState, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(AGDPlayerState, AttributePoints, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(AGDPlayerState, SpellPoints, COND_None, REPNOTIFY_Always);
}

void AGDPlayerState::SetLevel(int32 InLevel)
{
    if (InLevel == Level || InLevel < 0)
	{
		UE_LOG(LogGD, Warning, TEXT("%hs\t NewLevel Error, NewLevel[%d]"), __FUNCTION__, InLevel);
		return;
	}
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

void AGDPlayerState::AddToLevel(int32 InLevel)
{
    if (InLevel == 0)
	{
		UE_LOG(LogGD, Warning, TEXT("%hs\t InLevel Can't Equals Zero!"), __FUNCTION__);
		return;
	}
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

int32 AGDPlayerState::GetAttributePoints() const
{
	return AttributePoints;
}

int32 AGDPlayerState::GetSpellPoints() const
{
	return SpellPoints;
}

void AGDPlayerState::SetAttributePoints(int32 InPoints)
{
	if (FCheckTool::CheckAndSet(AttributePoints, InPoints))
	{
		OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
	}
}

void AGDPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AGDPlayerState::SetSpellPoints(int32 InPoints)
{
	if (FCheckTool::CheckAndSet(SpellPoints, InPoints))
	{
		OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
	}
}

void AGDPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

float AGDPlayerState::GetHealth() const
{
	if (AS) {
		return AS->GetHealth();
	}
	return 0.f;
}

float AGDPlayerState::GetMaxHealth() const
{
	if (AS) {
		return AS->GetMaxHealth();
	}
	return 0.f;
}

float AGDPlayerState::GetMana() const
{
	if (AS) {
		return AS->GetMana();
	}
	return 0.f;
}

float AGDPlayerState::GetMaxMana() const
{
	if (AS) {
		return AS->GetMaxMana();
	}
	return 0.f;
}

float AGDPlayerState::GetStamina() const
{
	if (AS) {
		return AS->GetStamina();
	}
	return 0.f;
}

float AGDPlayerState::GetMaxStamina() const
{
	if (AS) {
		return AS->GetMaxStamina();
	}
	return 0.f;
}

void AGDPlayerState::SetXP(int32 InXP)
{
	if (InXP == XP || InXP < 0)
	{
		UE_LOG(LogGD, Warning, TEXT("%hs\t NewXP Error, NewXP[%d]"), __FUNCTION__, InXP);
		return;
	}
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AGDPlayerState::AddToXP(int32 InXP)
{
	if (InXP == 0)
	{
		UE_LOG(LogGD, Warning, TEXT("%hs\t Addition Can't Equals Zero!"), __FUNCTION__);
		return;
	}
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void AGDPlayerState::OnRep_Level(const int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AGDPlayerState::OnRep_XP(const int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AGDPlayerState::OnRep_AttributePoints(const int32 OldPoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AGDPlayerState::OnRep_SpellPoints(const int32 OldPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
