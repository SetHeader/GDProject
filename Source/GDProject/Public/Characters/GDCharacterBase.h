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

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "GDCharacterBase")
	TObjectPtr<USkeletalMeshComponent> WeaponComponent;

	/// @brief 初始时 拥有的能力
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GDCharacterBase")
	TArray<TSubclassOf<UGameplayAbility>> SetupAbilities;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UGDAttributeSetBase> AS;

	UPROPERTY(EditDefaultsOnly, Category="GDCharacterBase")
	FName WeaponTipSocketName;

	UPROPERTY(EditDefaultsOnly, Category="GDCharacterBase")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDCharacterBase")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDCharacterBase")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GDCharacterBase")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	/* 角色死亡时 溶解效果 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GDCharacterBase")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GDCharacterBase")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	void Dissolve();

	/**
	 * 开始随时间线溶解，蓝图比较好实现
	 * @param MaterialInstanceDynamic 材质 
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);

	/**
	 * 开始随时间线溶解，蓝图比较好实现
	 * @param MaterialInstanceDynamic 材质 
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);
public:
	virtual void PossessedBy(AController* NewController) override;

	UFUNCTION(BlueprintCallable, Category = "GDCharacterBase")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC; }
	UFUNCTION(BlueprintCallable, Category = "GDCharacterBase")
	virtual UGDAttributeSetBase* GetGDASBase() const { return AS; };

	void AddCharacterAbilities();

	virtual UAnimMontage* GetHitReactMontage_Implementation() override;

	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
protected:
	virtual void InitializeAttributes() const;
};
