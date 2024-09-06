
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
	}
	 
	// 如果是Saving就会存储RepBits，如果是Loading就会读取到RepBits。
	Ar.SerializeBits(&RepBits, 2);

	if (Ar.IsLoading())
	{
		if (RepBits & 1 << 0)
		{
			SetIsBlockedHit(true);
		}
		
		if (RepBits & 1 << 1)
		{
			SetIsCriticalHit1(true);
		}
	}
	
	bOutSuccess = true;
	return true;
}
