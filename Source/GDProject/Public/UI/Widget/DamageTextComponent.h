// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 伤害文本组件
 * 动态创建后，附加到敌人上面，再调SetDamageText，就能显示出伤害
 */
UCLASS()
class GDPROJECT_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	/**
	 * 设置伤害文本的数值
	 * @param Damage 伤害 
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
};
