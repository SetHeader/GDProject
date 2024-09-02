// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "GDPlayerState.generated.h"

class UGDAttributeSetBase;
struct FOnAttributeChangeData;
struct FGDASBase;

/**
 * 
 */
UCLASS()
class GDPROJECT_API AGDPlayerState : public APlayerState, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "GDCharacterBase")
	TObjectPtr<UAbilitySystemComponent> ASC;

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase")
	TObjectPtr<UGDAttributeSetBase> AS;

protected:
	UPROPERTY(ReplicatedUsing = OnRep_Level)
	int32 Level = 1;

public:
	AGDPlayerState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "GDCharacterBase")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	UFUNCTION(BlueprintCallable, Category = "GDCharacterBase")
	virtual UGDAttributeSetBase* GetGDASBase() const { return AS; };

	UFUNCTION(BlueprintCallable, Category = "CombatInterface")
	FORCEINLINE int32 GetPlayerLevel() const override { return Level; }

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

	UFUNCTION()
	virtual void OnRep_Level(const int32 OldLevel);
};
