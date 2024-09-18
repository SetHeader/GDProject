// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AsyncTasks/WaitCooldownChange.h"
#include "AbilitySystemComponent.h"

UWaitCooldownChange* UWaitCooldownChange::WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                                const FGameplayTag& CooldownTag)
{
	UWaitCooldownChange* WaitCooldownChange = NewObject<UWaitCooldownChange>();

	WaitCooldownChange->ASC = AbilitySystemComponent;
	WaitCooldownChange->CooldownTag = CooldownTag;
	
	if (!IsValid(AbilitySystemComponent) || !CooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	// 监听冷却效果的结束（冷却标签被移除就是效果结束了）
	AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(WaitCooldownChange, &UWaitCooldownChange::CooldownTagChanged);
	// 监听冷却效果的应用
	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf
		.AddUObject(WaitCooldownChange, &UWaitCooldownChange::OnEffectApplied);
	
	return WaitCooldownChange;
}

void UWaitCooldownChange::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		
		SetReadyToDestroy();
		MarkAsGarbage();
	}
}

void UWaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UWaitCooldownChange::OnEffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer AssetTags;
	EffectSpec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectSpec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> RemainingTimes = ASC->GetActiveEffectsTimeRemaining(Query);
		if (RemainingTimes.Num() > 0)
		{
			// 找出最长的剩余时间，再广播出去
			float HighestRemainingTime = RemainingTimes[0];
			for (int32 i = 0; i < RemainingTimes.Num(); i++)
			{
				if (RemainingTimes[i] > HighestRemainingTime)
				{
					HighestRemainingTime = RemainingTimes[i];
				}
			}
			
			CooldownStart.Broadcast(HighestRemainingTime);
		}
	}
}
