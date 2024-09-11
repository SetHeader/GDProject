// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValidDataSignature, const FGameplayAbilityTargetDataHandle&, DataDataHandle);

/**
 * 用于客户端向服务端发送鼠标位置信息
 */
UCLASS()
class GDPROJECT_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	/**
	 * 该节点在客户端、服务端都会广播。
	 */
	UPROPERTY(BlueprintAssignable, Category="Tasks")
	FOnValidDataSignature OnValidDataDelegate;

	/**
	 * 对外暴露的方法，会隐藏输入参数 OwningAbility 
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", BlueprintInternalUseOnly,
		meta=(DisplayName = "TargetDataUnderMouse", HidePin="OwningAbility", DefaultToSelf="OwningAbility"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;

	// 在客户端调用，用来向服务端发送鼠标信息
	void SendMouseCursorData();

	// 在服务端调用，当服务端收到客户端信息的时候会执行
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
