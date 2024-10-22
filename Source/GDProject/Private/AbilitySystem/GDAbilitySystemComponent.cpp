// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDAbilitySystemComponent.h"
#include <AbilitySystemComponent.h>

#include "AbilitySystemBlueprintLibrary.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/GDGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/LoadScreenSaveGame.h"
#include "GDProject/GDLogChannels.h"
#include "Interaction/PlayerInterface.h"
#include "Player/GDPlayerState.h"


void UGDAbilitySystemComponent::OnAbilityActorInfoSet()
{
	// 在服务端和客户端都会绑定，但只会在服务端回调。但回调方法ClientEffectApplied是个rpc方法，会在客户端执行。
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UGDAbilitySystemComponent::ClientEffectApplied);
}

void UGDAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData)
{
	FGDGameplayTags& GameplayTags = FGDGameplayTags::Get();
	for (const FSavedAbility& SavedAbility : SaveData->SavedAbilities)
	{
		// 只有解锁且可用才能授予能力
		if (SavedAbility.AbilityStatus != GameplayTags.Abilities_Status_Unlocked && SavedAbility.AbilityStatus != GameplayTags.Abilities_Status_Equipped)
		{
			return;
		}
		// 能力等级检查
		if (SavedAbility.AbilityLevel <= 0)
		{
			return;
		}
		// 能力检查
		if (!SavedAbility.GameplayAbility)
		{
			return;
		}
		
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(SavedAbility.GameplayAbility, SavedAbility.AbilityLevel);
		AbilitySpec.DynamicAbilityTags.AddTag(SavedAbility.AbilityStatus);
		AbilitySpec.DynamicAbilityTags.AddTag(SavedAbility.AbilitySlot);
		if (UGDGameplayAbility* GDAbility = Cast<UGDGameplayAbility>(AbilitySpec.Ability))
		{
			GDAbility->SetupInputTag = SavedAbility.AbilitySlot;
			AbilitySpec.DynamicAbilityTags.AddTag(SavedAbility.AbilitySlot);
		}
		
		// 主动能力就授予，被动能力就激活一次。
		if (SavedAbility.AbilityType == GameplayTags.Abilities_Type_Offensive)
		{
			GiveAbility(AbilitySpec);
		}
		else if (SavedAbility.AbilityType == GameplayTags.Abilities_Type_Passive)
		{
			GiveAbilityAndActivateOnce(AbilitySpec);
			
		}
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UGDAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : SetupAbilities)
	{
		FGameplayAbilitySpec Ability = FGameplayAbilitySpec(AbilityClass, 1);
		if (UGDGameplayAbility* GDAbility = Cast<UGDGameplayAbility>(Ability.Ability))
		{
			Ability.DynamicAbilityTags.AddTag(GDAbility->SetupInputTag);
		}
		GiveAbility(Ability);
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UGDAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : SetupPassiveAbilities)
	{
		FGameplayAbilitySpec Ability = FGameplayAbilitySpec(AbilityClass, 1);
		Ability.DynamicAbilityTags.AddTag(FGDGameplayTags::Get().Abilities_Status_Equipped);
		GiveAbilityAndActivateOnce(Ability);
	}
}

void UGDAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

		/*
		 1、GDCharacterHero->OnRep_PlayerState()
		 1.1、HUD->InitOverlay
		 1.1.1、OverlayWidgetController->SetParams
		 1.1.1.1、OverlayWidgetController->BindCallbacksToDependencies
		 1.1.1.1.1、如果能力初始化完成，就回调给组件蓝图；否则就监听
						if (GDASC->bStartupAbilitiesGiven)
						{
							OnInitializeStartupAbilities(GDASC);
						}
						else
						{
							GDASC->AbilitiesGivenDelegate.AddUObject(this, &UGDOverlayWidgetController::OnInitializeStartupAbilities);
						}

		 1.1.1.2、OverlayWidget->SetWidgetController(OverlayWidgetController)
		 1.1.1.2.1、OnWidgetControllerSet，触发蓝图中的逻辑，即绑定OnAbilityInfoDelegate，监听能力是否被给予
		 ......
		 2、GDAbilitySystemComponent->OnRep_ActivateAbilities()
		 2.1、GDAbilitySystemComponent->AbilitiesGivenDelegate.Broadcast(this)	广播能力初始化完成
			分支1：当OverlayWidgetController没有初始化时，是没反应的。
			等到OverlayWidgetController初始化后，会直接执行OnInitializeStartupAbilities，而不是监听GDASC->AbilitiesGivenDelegate
				2.1.1、OverlayWidgetController->OnInitializeStartupAbilities
				2.1.1.1、OnAbilityInfoDelegate.Broadcast(AbilityInfo)	回调所有已初始化的能力给蓝图
			分支2：正常情况下OverlayWidgetController已经初始化了。
				2.1.1、OverlayWidgetController->OnInitializeStartupAbilities
				2.1.1.1、OnAbilityInfoDelegate.Broadcast(AbilityInfo)	回调所有已初始化的能力给蓝图
			
		 以上流程中，GDCharacterHero->OnRep_PlayerState() 和 GDAbilitySystemComponent->OnRep_ActivateAbilities() 哪个先回调是不确定的，
		 故在OverlayWidgetController->BindCallbacksToDependencies中需要判断是否已经初始化了能力。
		 所有情况都会回调 OnAbilityInfoDelegate.Broadcast(AbilityInfo) 给组件蓝图。
		*/
	
	// TODO 有一个问题：回调给组件蓝图的时机（OnAbilityInfoDelegate.Broadcast） 和 组件蓝图初始化的时机（OnWidgetControllerSet） 不固定。 
	//		可能会先回调给组件蓝图，再执行组件蓝图初始化，这样就导致组件蓝图收不到能力广播。			
	//		在先执行流程2，再执行海程1时，可能会出现。但实际测试下来，永远是先执行流程1，再执行流程2。	
	//		故先不处理。
	
	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
}

void UGDAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	FGameplayTagContainer TagContainer;
	GESpec.GetAllAssetTags(TagContainer);
	if (!TagContainer.IsEmpty()) {
		OnEffectTagApplied.Broadcast(TagContainer);
	}
} 


void UGDAbilitySystemComponent::OnAbilityInputPressed(FGameplayTag InputTag)
{	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
}

void UGDAbilitySystemComponent::OnAbilityInputReleased(FGameplayTag InputTag)
{	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

void UGDAbilitySystemComponent::OnAbilityInputHeld(FGameplayTag InputTag)
{	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (!AbilitySpec.IsActive())
			{
				AbilitySpecInputPressed(AbilitySpec);
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UGDAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	// 锁住能力列表，防止遍历过程中 可激活的能力 变成 不可激活
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogGD, Warning, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UGDAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (UGameplayAbility* Ability = AbilitySpec.Ability)
	{
		const FGameplayTag& AbilityTagPrefix = FGameplayTag::RequestGameplayTag("Abilities");
		for (const FGameplayTag& Tag : Ability->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTagPrefix))
			{
				return Tag;
			}
		}
	}
	
	return FGameplayTag();
}

FGameplayTag UGDAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	// 输入标签是加到DynamicAbilityTags中的，所以遍历这个就行
	const FGameplayTag& InputTagPrefix = FGameplayTag::RequestGameplayTag("InputTag");
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(InputTagPrefix))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UGDAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	// 状态标签是加到DynamicAbilityTags中的，所以遍历这个就行
	const FGameplayTag& InputTagPrefix = FGameplayTag::RequestGameplayTag("Abilities.Status");
	for (const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(InputTagPrefix))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UGDAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& ActivatableAbility : GetActivatableAbilities())
	{
		if (ActivatableAbility.Ability->AbilityTags.HasTagExact(AbilityTag))
		{
			return &ActivatableAbility;
		}
	}

	return nullptr;
}

FGameplayTag UGDAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}
	return FGameplayTag();
}

FGameplayTag UGDAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag();
}

void UGDAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot)
{
	ClearSlot(&Spec);
	Spec.DynamicAbilityTags.AddTag(Slot);
}

void UGDAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
}

void UGDAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UGDAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTagExact(Slot))
		{
			return true;
		}
	}
	return false;
}

void UGDAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
	bool bActivate)
{
	ActivatePassiveEffect.Broadcast(AbilityTag, bActivate);
}

void UGDAbilitySystemComponent::UpgradeAttributePoint(const FGameplayTag& Tag)
{
	if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
	{
		ServerUpgradeAttributePoint(Tag);
	}
}

void UGDAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	UAbilityInfo* AbilityInfo = UGDAbilitySystemLibrary::GetAbilityInfo(this);
	for (const FGDAbilityInfo& Info : AbilityInfo->AbilityInfos)
	{
		if (!Info.AbilityTag.IsValid())
		{
			continue;
		}
		if (Info.LevelRequirement > Level)
		{
			continue;
		}
		// 如果该能力还没被给予，就给予能力
		if (!GetSpecFromAbilityTag(Info.AbilityTag))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FGDGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			// 立即强制复制能力到客户端，而不是等到下次更新时机
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FGDGameplayTags::Get().Abilities_Status_Eligible, AbilitySpec.Level);
		}
		
	}
}

void UGDAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	GetSpecFromAbilityTag(AbilityTag);
}

void UGDAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
                                                                         const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	OnAbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UGDAbilitySystemComponent::ServerUpgradeAttributePoint_Implementation(const FGameplayTag& Tag)
{
	FGameplayEventData Payload;
	Payload.EventTag = Tag;
	Payload.EventMagnitude = 1;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), Tag, Payload);

	IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
}


void UGDAbilitySystemComponent::ServerUpgradeAbility_Implementation(FGameplayTag AbilityTag)
{
	if (UAbilityInfo* AbilityInfo = UGDAbilitySystemLibrary::GetAbilityInfo(GetOwner()))
	{
		FGDAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		AGDPlayerState* GDPlayerState = CastChecked<AGDPlayerState>(GetOwner());
		
		if (GDPlayerState->GetPlayerLevel() >= Info.LevelRequirement && GDPlayerState->GetSpellPoints() > 0)
		{
			FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
			
			// 升级能力
			if (AbilitySpec)
			{
				int32 AbilityLevel = AbilitySpec->Ability->GetAbilityLevel() + 1;
				if (AbilityLevel < 9)
				{
					GDPlayerState->AddToSpellPoints(-1);
					ClearAbility(AbilitySpec->Handle);
					GiveAbility(FGameplayAbilitySpec(Info.Ability, AbilityLevel));
				}
			}
			// 解锁能力
			else
			{
				GiveAbility(FGameplayAbilitySpec(Info.Ability, 1));
				ClientUpdateAbilityStatus(AbilityTag, FGDGameplayTags::Get().Abilities_Status_Unlocked, 1);
			}
		}
	}
}

void UGDAbilitySystemComponent::ServerEquipAbility_Implementation(FGameplayTag AbilityTag, FGameplayTag InputTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	UGDGameplayAbility* GDAbility = Cast<UGDGameplayAbility>(AbilitySpec->Ability);
	if (GDAbility)
	{
		if (GDAbility->SetupInputTag != InputTag)
		{
			GDAbility->SetupInputTag = InputTag;
			ClientUpdateAbilityStatus(AbilityTag, FGDGameplayTags::Get().Abilities_Status_Equipped, GDAbility->GetAbilityLevel());
		}
	}
}