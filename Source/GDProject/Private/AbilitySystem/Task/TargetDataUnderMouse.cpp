// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Task/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* TargetDataUnderMouse = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return TargetDataUnderMouse;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	// 客户端发送鼠标数据，服务端监听并处理
	if (IsLocallyControlled())
	{
		SendMouseCursorData();
	} else
	{
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey PredictionKey = GetActivationPredictionKey();
		
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey)
			.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		
		AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	if (HitResult.bBlockingHit)
	{
		FGameplayAbilityTargetDataHandle DataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
		Data->HitResult = HitResult;
		DataHandle.Add(Data);
		AbilitySystemComponent->ServerSetReplicatedTargetData(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey(),
			DataHandle,
			FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnValidDataDelegate.Broadcast(DataHandle);
		}
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnValidDataDelegate.Broadcast(DataHandle);
	}
}
