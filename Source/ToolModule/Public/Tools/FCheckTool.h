#pragma once

struct FCheckTool
{
	FORCEINLINE static bool CheckAndSet(int& ValueToSet, const int NewValue)
	{
		if (ValueToSet == NewValue || NewValue <= 0)
		{
			UE_LOG(LogGD, Warning, TEXT("%hs\t Ignore Set Value. OldValue[%d]  NewValue[%d]"), __FUNCTION__, ValueToSet, NewValue)
			return false;
		}

		ValueToSet = NewValue;
		return true;
	}

	FORCEINLINE static bool CheckAndAdd(int& ValueToAdd, const int NewValue)
	{
		if (NewValue <= 0)
		{
			UE_LOG(LogGD, Warning, TEXT("%hs\t Ignore Add Value. OldValue[%d]  NewValue[%d]"), __FUNCTION__, ValueToAdd, NewValue)
			return false;
		}

		ValueToAdd += NewValue;
		return true;
	}
};
