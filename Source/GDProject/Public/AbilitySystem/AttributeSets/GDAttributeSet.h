
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GDAttributeSet.generated.h"


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
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	AController* SourceController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	ACharacter* SourceCharacter = nullptr;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	AController* TargetController = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectProperties")
	ACharacter* TargetCharacter = nullptr;
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
 * 基础属性集
 */
UCLASS()
class GDPROJECT_API UGDAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UGDAttributeSet();
	
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> Tag2AttributeFuncMap;

	// 主要属性，Primary Attributes
	// 力量
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Strength)
	// 智力
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Intelligence)
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Intelligence)
	// 韧性
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Resilience)
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Resilience)
	// 活力
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Vigor)
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Vigor)


	// 次要属性，Secondary Attributes
	// 护甲点数
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Armor)
	// 护甲穿透点数
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_ArmorPenetration)
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, ArmorPenetration)
	// 格挡成功率，范围：0-100
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_BlockChange)
	FGameplayAttributeData BlockChange;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, BlockChange)
	// 致命一击率（暴击率），暴击造成2倍伤害，再加上暴击额外伤害
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_CriticalHitChange)
	FGameplayAttributeData CriticalHitChange;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, CriticalHitChange)
	// 致命一击额外伤害
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_CriticalHitDamage)
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, CriticalHitDamage)
	// 致命一击抵抗点数
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_CriticalHitResistance)
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, CriticalHitResistance)
	// 每秒生命回复点数
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_HealthRegeneration)
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, HealthRegeneration)
	// 每秒魔力回复点数
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_ManaRegeneration)
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, ManaRegeneration)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_FireResistance)
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, FireResistance)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_LightingResistance)
	FGameplayAttributeData LightingResistance;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, LightingResistance)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_ArcaneResistance)
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, ArcaneResistance)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_PhysicalResistance)
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, PhysicalResistance)

	// 重要属性，Vital Attributes
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, Stamina)

	/**
	 * 元属性，Meta Attribute
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, IncomingDamage)
	
	UPROPERTY(BlueprintReadOnly, Category = "GDAttributeSetBase")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UGDAttributeSet, IncomingXP)

private:
	// 是否要补满血量
	bool bTopOffHealth = false;
	// 是否要补满魔力
	bool bTopOffMana = false;
public:

	// 属性变化前回调
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 执行效果后回调
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	// 属性更新完成后回调
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	// 复制
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
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

	UFUNCTION()
	virtual void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance);

	UFUNCTION()
	virtual void OnRep_LightingResistance(const FGameplayAttributeData& OldLightingResistance);

	UFUNCTION()
	virtual void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance);

	UFUNCTION()
	virtual void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance);

private:
	void SendXPEvent(const FEffectProperties& Props);

};