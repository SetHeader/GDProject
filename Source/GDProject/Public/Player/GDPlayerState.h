// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GDPlayerState.generated.h"

class ULevelUpInfo;
class UGDAttributeSet;
struct FOnAttributeChangeData;
struct FGDASBase;
struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE_OneParam(FStatChangedSignature, const int32 /* NewStat */);

/**
 * 玩家状态类，储存了玩家信息，提供了GAS系统支持
 */
UCLASS()
class GDPROJECT_API AGDPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "GDCharacterBase")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase")
	TObjectPtr<UGDAttributeSet> AS;
	// 角色等级所需经验配置
	UPROPERTY(EditDefaultsOnly, Category = "GDCharacterBase")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FStatChangedSignature OnLevelChangedDelegate;
	FStatChangedSignature OnXPChangedDelegate;
	FStatChangedSignature OnAttributePointsChangedDelegate;
	FStatChangedSignature OnSpellPointsChangedDelegate;
protected:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints;
	UPROPERTY(ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints;
public:
	AGDPlayerState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "GDCharacterBase")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	UFUNCTION(BlueprintCallable, Category = "GDCharacterBase")
	virtual UGDAttributeSet* GetGDASBase() const { return AS; };

	UFUNCTION(BlueprintCallable, Category = "CombatInterface")
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	
	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	void SetLevel(int32 InLevel);

	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	void AddToLevel(int32 InLevel);

	UFUNCTION()
	FORCEINLINE int32 GetAttributePoints() const;
	
	UFUNCTION()
	void SetAttributePoints(int32 InPoints);

	UFUNCTION()
	void AddToAttributePoints(int32 InPoints);

	UFUNCTION()
	FORCEINLINE int32 GetSpellPoints() const;
	
	UFUNCTION()
	void SetSpellPoints(int32 InPoints);

	UFUNCTION()
	void AddToSpellPoints(int32 InPoints);
	
	/* 获取AS中的属性 */

	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	float GetMana() const;
	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	float GetMaxMana() const;
	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	float GetStamina() const;
	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	FORCEINLINE int32 GetXP() const { return XP; }
	
	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	void SetXP(int32 InXP);

	UFUNCTION(BlueprintCallable, Category = "GDAttributeSetBase")
	void AddToXP(int32 InXP);
	
	UFUNCTION()
	virtual void OnRep_Level(const int32 OldLevel);

	UFUNCTION()
	virtual void OnRep_XP(const int32 OldXP);

	UFUNCTION()
	virtual void OnRep_AttributePoints(const int32 OldPoints);

	UFUNCTION()
	virtual void OnRep_SpellPoints(const int32 OldPoints);
};
