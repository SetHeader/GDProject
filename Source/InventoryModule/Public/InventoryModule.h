#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * 物品、背包、装备模块
 * InventoryComponent: 物品仓库组件，能存放、拿出物品，能对物品进行操作
 * ItemDefinition：物品的定义，类似 Class
 * ItemInstance：物品的实例，类似 实例化的对象
 * Fragment：物品属性，如IconFragment、EquipFragment。在物品定义中，可以有多个物品属性。
 * 
 */