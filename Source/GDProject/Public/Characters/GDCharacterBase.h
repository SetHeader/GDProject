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
class UDebuffNiagaraComponent;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;
	
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

	// 燃烧debuff时的特效
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;
	// 眩晕debuff时的特效
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;
	
	/* 随从数量 */
	int32 MinionCount = 0;

	UPROPERTY(BlueprintReadOnly)
	bool bIsBurning = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsStunned = false;
	// 是否可以移动
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IsCanMove)
	bool bCanMove = true;
	// 是否可以攻击
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_IsCanAttack)
	bool bCanAttack = true;

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

	FOnASCRegistered OnAscRegistered;
	FOnDeathSignature OnDeathDelegate;
	FOnDamageSignature OnDamageDelegate;
	
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

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 添加初始能力
	void AddSetupAbilities();
	// 添加初始被动能力
	void AddSetupPassiveAbilities();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	/** Combat Interface */
	FVector GetCombatSocketLocation_Implementation(const FGameplayTag& CombatSocketTag);
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	bool IsDead_Implementation() const override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual FOnDamageSignature& GetOnDamageDelegate() override;
	AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetTaggedMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
	virtual int32 GetMinionCount_Implementation() const override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	USkeletalMeshComponent* GetWeapon_Implementation();
	/** End Combat Interface */
	
	UFUNCTION()
	virtual void OnRep_IsCanMove();

	UFUNCTION()
	virtual void OnRep_IsCanAttack();
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

protected:
	/**
	 * 当ASC组件可用时回调，需要子类实现
	 */
	UFUNCTION()
	virtual void OnAbilitySystemComponentAvailable();

	/**
	 * 统一处理标签变化的方法
	 * @param Tag 标签
	 * @param Count 标签数量
	 */
	virtual void OnGameplayTagChanged(const FGameplayTag Tag, int32 Count);
	
	// 初始化默认属性
	virtual void InitializeDefaultAttributes() const;
};
