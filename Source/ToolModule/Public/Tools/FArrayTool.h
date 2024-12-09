#pragma once

#include "CoreMinimal.h"

struct FArrayTool
{
	template <typename T>
	static void RemoveIf(TArray<T> Array, TFunction<bool(T)> NeedRemove)
	{
		TArray<T> Temp;
		for (auto& Value : Array)
		{
			if (NeedRemove(Value))
			{
				Temp.Add(Value);
			}
		}

		for (auto& Value : Temp)
		{
			Array.Remove(Value);
		}
	}
};
