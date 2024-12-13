#pragma once

#include "EquipmentManagerInferface.generated.h"

class UEquipmentManagerComponent;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UEquipmentManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class INVENTORYMODULE_API IEquipmentManagerInterface
{
	GENERATED_IINTERFACE_BODY()

	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent() const = 0;
};
