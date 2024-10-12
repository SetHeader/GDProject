
#include "AbilitySystem/GDAbilityTypes.h"

#include "Serialization/BufferArchive.h"


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
	
	uint8 RepBits = 0;
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
	}
	 
	// 如果是Saving就会存储RepBits，如果是Loading就会读取到RepBits。
	Ar.SerializeBits(&RepBits, 7);

	if (Ar.IsLoading())
	{
		if (RepBits & (1 << 0))
		{
			SetIsBlockedHit(true);
		}
		
		if (RepBits & (1 << 1))
		{
			SetIsCriticalHit(true);
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
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}
	
	bOutSuccess = true;
	return true;
}
