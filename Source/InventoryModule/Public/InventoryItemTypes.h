#pragma once

UENUM(BlueprintType)
enum class EInventoryItemType : uint8 {
	// 普通物品
	Common,
	// 可以装备的物品
	Equipment,
	// 可以使用的物品
	Usable
};