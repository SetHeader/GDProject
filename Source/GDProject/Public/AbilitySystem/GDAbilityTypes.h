#pragma once

#include "GameplayEffectTypes.h"
#include "GDAbilityTypes.generated.h"

class UGameplayEffect;

/**
 * 伤害效果参数
 */
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;
	
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	// TODO 这个属性没有用到，或许应该在EffectContext上也加上击退概率，然后在UAttributeSet::PostGameplayEffectExecute方法里面处理
	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	// 是否为径向伤害，即范围伤害，伤害随圆心距离而递减
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;

	// 径向伤害的内圆半径，在内圆半径内会受到满伤害
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	// 径向伤害的外圆半径，在外圆半径时受到最小伤害
	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	// 径向伤害的圆心
	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;
};

USTRUCT(BlueprintType)
struct FGDGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
	
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	TSharedPtr<FGameplayTag> DamageType;
	
	UPROPERTY()
	bool bIsRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;
public:
	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FGDGameplayEffectContext* Duplicate() const override;
	
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	FORCEINLINE bool IsBlockedHit() const { return bIsBlockedHit; }
	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }
	
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetKnockbackForce() const { return KnockbackForce; }

	[[nodiscard]] bool IsRadialDamage() const { return bIsRadialDamage; }
	[[nodiscard]] float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	[[nodiscard]] float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	[[nodiscard]] FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	
	void SetIsBlockedHit(bool BlockedHit) { bIsBlockedHit = BlockedHit; }
	void SetIsCriticalHit(bool CriticalHit) { bIsCriticalHit = CriticalHit; }
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetKnockbackForce(const FVector& InForce) { KnockbackForce = InForce; }

	void SetIsRadialDamage(bool InIsRadialDamage) { this->bIsRadialDamage = InIsRadialDamage; }
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { this->RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { this->RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { this->RadialDamageOrigin = InRadialDamageOrigin; }

	FVector GetDeathImpulse() const { return DeathImpulse; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { this->DeathImpulse = InDeathImpulse; }
};

template<>
struct TStructOpsTypeTraits<FGDGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FGDGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};