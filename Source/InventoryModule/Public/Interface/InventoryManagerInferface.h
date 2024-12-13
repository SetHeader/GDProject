#pragma once

#include "InventoryManagerInferface.generated.h"

class UInventoryManagerComponent;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInventoryManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class INVENTORYMODULE_API IInventoryManagerInterface
{
	GENERATED_IINTERFACE_BODY()

	virtual UInventoryManagerComponent* GetInventoryManagerComponent() const = 0;
};
