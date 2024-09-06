#pragma once

#include "GameplayEffectTypes.h"
#include "GDAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FGDGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
	
public:
	virtual UScriptStruct* GetScriptStruct() const override;

	virtual FGDGameplayEffectContext* Duplicate() const override;
	
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	FORCEINLINE bool IsBlockedHit() const { return bIsBlockedHit; }
	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }

	void SetIsBlockedHit(bool BlockedHit) { bIsBlockedHit = BlockedHit; }
	void SetIsCriticalHit1(bool CriticalHit) { bIsCriticalHit = CriticalHit; }
};

template<>
struct TStructOpsTypeTraits<FGDGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FGDGameplayEffectContext>
{
	enum
	{
		WidthNetSerializer = true,
		WithCopy = true
	};
};