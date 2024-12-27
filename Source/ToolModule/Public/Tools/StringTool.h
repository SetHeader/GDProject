#pragma once

#include "CoreMinimal.h"

TOOLMODULE_API struct StringTool {
public:
	static FString Stringify(ENetMode NM);
	static FString Stringify(ENetRole NR);
};
