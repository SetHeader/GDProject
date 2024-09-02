// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "GDCharacterBase.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class UGDAttributeSetBase;
class UGameplayAbility;
class UGameplayEffect;

/**
* 角色基类，提供 GAS 接口
*/
UCLASS()
class GDPROJECT_API AGDCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AGDCharacterBase();

	UPROPERTY(BlueprintReadOnly, Category = "GDCharacterBase")
	TObjectPtr<USkeletalMeshComponent> WeaponComponent;

	/// @brief 初始时 拥有的能力
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GDCharacterBase")
	TArray<TSubclassOf<UGameplayAbility>> SetupAbilities;

	TObjectPtr<UAbilitySystemComponent> ASC;
	TObjectPtr<UGDAttributeSetBase> AS;

	UPROPERTY(EditDefaultsOnly, Category="GDCharaterBase")
	FName WeaponTipSocketName;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDCharacterBase")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDCharacterBase")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDCharacterBase")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
public:
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "GDCharacterBase")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	UFUNCTION(BlueprintCallable, Category = "GDCharacterBase")
	virtual UGDAttributeSetBase* GetGDASBase() const { return AS; };

	void AddCharacterAbilities();
protected:
	void InitializeAttributes() const;
};
