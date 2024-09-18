// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "GDCharacterBase.generated.h"

struct FEffectProperties;
class UNiagaraSystem;
class UGameplayAbility;
class UAbilitySystemComponent;
class UGDAttributeSet;
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

	/// @brief 初始时 拥有的能力，在初始化时会给予能力
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GDCharacterBase")
	TArray<TSubclassOf<UGameplayAbility>> SetupAbilities;
	/// @brief 初始时 拥有的被动能力，在初始化时会给予并激活一次
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GDCharacterBase")
	TArray<TSubclassOf<UGameplayAbility>> SetupPassiveAbilities;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;
	UPROPERTY()
	TObjectPtr<UGDAttributeSet> AS;
	// 武器尖端插槽名
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	FName TailSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	FName RightHandSocketName;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMongages;

	UPROPERTY(EditAnywhere, Category="Combat")
	UNiagaraSystem* BloodEffect;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	USoundBase* DeadSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	/* Minions */
	int32 MinionCount = 0;
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

	bool bIsDead = false;
	
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
	virtual UGDAttributeSet* GetGDASBase() const { return AS; };

	void AddCharacterAbilities();

	/** Combat Interface */
	FVector GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag);
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	bool IsDead_Implementation() const override;
	AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetTaggedMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
	virtual int32 GetMinionCount_Implementation() const override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	/** End Combat Interface */
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

protected:
	virtual void InitializeAttributes() const;
};
