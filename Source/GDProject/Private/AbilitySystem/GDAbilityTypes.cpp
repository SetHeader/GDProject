
#include "AbilitySystem/GDAbilityTypes.h"


UScriptStruct* FGDGameplayEffectContext::GetScriptStruct() const
{
	return StaticStruct();
}

FGDGameplayEffectContext* FGDGameplayEffectContext::Duplicate() const
{
	FGDGameplayEffectContext* NewContext = new FGDGameplayEffectContext();
	*NewContext = *this;
	if (GetHitResult())
	{
		// Does a deep copy of the hit result
		NewContext->AddHitResult(*GetHitResult(), true);
	}
	return NewContext;
}

bool FGDGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// 每一位表示一个属性是否有值
	uint16 RepBits = 0;
	if (Ar.IsSaving()) {
		if (IsBlockedHit())
		{
			RepBits |= 1 << 0;
		}
		if (IsCriticalHit())
		{
			RepBits |= 1 << 1;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 2;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 3;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 4;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 5;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 6;
		}
		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 7;
		}
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 8; 
		}
		
		if (RadialDamageInnerRadius > 0) {
			RepBits |= 1 << 9;
		}
		
		if (RadialDamageOuterRadius > 0) {
			RepBits |= 1 << 10;
		}

		if (!RadialDamageOrigin.IsZero())
		{
			RepBits |= 1 << 11;
		}
	}
	 
	// 如果是Saving就会存储RepBits，如果是Loading就会读取到RepBits。
	Ar.SerializeBits(&RepBits, 12);
	
	if (RepBits & (1 << 0))
	{
		Ar << bIsBlockedHit;
	}
	
	if (RepBits & (1 << 1))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 2))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 3))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 4))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 5))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 6))
	{
		if (!DamageType.IsValid())
		{
			DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
		}
		
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 7))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsRadialDamage;
	}
	if (RepBits & (1 << 9))
	{
		Ar << RadialDamageInnerRadius;
	}
	if (RepBits & (1 << 10))
	{
		Ar << RadialDamageOuterRadius;
	}
	if (RepBits & (1 << 11))
	{
		Ar << RadialDamageOrigin;
	}
	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}
	
	
	bOutSuccess = true;
	return true;
}
