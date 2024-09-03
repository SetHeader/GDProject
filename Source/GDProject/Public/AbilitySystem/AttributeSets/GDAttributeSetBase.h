// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GDAttributeSetBase.generated.h"


// 使用这个宏来快速生成getter、setter等方法
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 封装常用对象，方便后续获取
 */
USTRUCT(BlueprintType)
struct FEffectProperties {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	AActor* SourceActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	AController* SourceController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	ACharacter* SourceCharacter = nullptr;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	AActor* TargetActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	AController* TargetController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	ACharacter* TargetCharacter = nullptr;
};


/// 基础属性枚举
UENUM(BlueprintType)
enum class EGDASBase : uint8
{
	Health,
	MaxHealth,
	Mana,
	MaxMana,
	Stamina,
	MaxStamina
};

// 属性结构，对应 UGDAttributeSetBase
USTRUCT(BlueprintType)
struct FGDASBase : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxMana = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Stamina = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxStamina = 0.f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedDelegate, const float, NewValue, const float, OldValue);

// 本质是函数指针
// TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FFuncPtr;

// 别名方式一
// using FAttributeFuncPtr = TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr;

// 别名方式二，更加通用
template<class Func>
using TStaticFuncPtr =  typename TBaseStaticDelegateInstance<Func, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class GDPROJECT_API UGDAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UGDAttributeSetBase();
	
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> Tag2AttributeFuncMap;

	// 属性变化前回调
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 执行效果后回调
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// 复制
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	// 主要属性，Primary Attributes
	// 力量
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Strength)
	// 智力
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Intelligence)
	// 韧性
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Resilience)
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Resilience)
	// 活力
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Vigor)
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Vigor)


	// 次要属性，Secondary Attributes
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Armor)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_ArmorPenetration)
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, ArmorPenetration)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_BlockChange)
	FGameplayAttributeData BlockChange;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, BlockChange)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_CriticalHitChange)
	FGameplayAttributeData CriticalHitChange;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, CriticalHitChange)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_CriticalHitDamage)
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, CriticalHitDamage)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_CriticalHitResistance)
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, CriticalHitResistance)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_HealthRegeneration)
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, HealthRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_ManaRegeneration)
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, ManaRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, MaxStamina)

	// 重要属性，Vital Attributes
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Health)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Stamina)

	/**
	 * Meta Attribute
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, IncomingDamage)

	/* 客户端属性变化通知 */

	UPROPERTY(BlueprintAssignable, Category = "GDAttributeSetBase")
	FOnAttributeChangedDelegate OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "GDAttributeSetBase")
	FOnAttributeChangedDelegate OnMaxHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "GDAttributeSetBase")
	FOnAttributeChangedDelegate OnManaChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "GDAttributeSetBase")
	FOnAttributeChangedDelegate OnMaxManaChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "GDAttributeSetBase")
	FOnAttributeChangedDelegate OnStaminaChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category = "GDAttributeSetBase")
	FOnAttributeChangedDelegate OnMaxStaminaChangedDelegate;

public:

	void SetEffectProperties(const FGameplayEffectModCallbackData& InData, FEffectProperties& OutProps);

	UFUNCTION()
	virtual void OnRep_Strength(const FGameplayAttributeData& OldStrength);
	UFUNCTION()
	virtual void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence);
	UFUNCTION()
	virtual void OnRep_Resilience(const FGameplayAttributeData& OldResilience);
	UFUNCTION()
	virtual void OnRep_Vigor(const FGameplayAttributeData& OldVigor);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);
	UFUNCTION()
	virtual void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration);
	UFUNCTION()
	virtual void OnRep_BlockChange(const FGameplayAttributeData& OldBlockChange);
	UFUNCTION()
	virtual void OnRep_CriticalHitChange(const FGameplayAttributeData& OldCriticalHitChange);
	UFUNCTION()
	virtual void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage);
	UFUNCTION()
	virtual void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance);
	UFUNCTION()
	virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration);
	UFUNCTION()
	virtual void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);
};