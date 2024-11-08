// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GDAbilitySystemComponent.h"
#include <AbilitySystemComponent.h>

#include "AbilitySystemBlueprintLibrary.h"
#include "GDGameplayTags.h"
#include "AbilitySystem/GDAbilitySystemLibrary.h"
#include "AbilitySystem/Abilities/GDGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Game/LoadScreenSaveGame.h"
#include "GDProject/GDLog.h"
#include "Interaction/PlayerInterface.h"
#include "Player/GDPlayerState.h"

void UGDAbilitySystemComponent::OnAbilityActorInfoSet()
{
	// 在服务端和客户端都会绑定，但只会在服务端回调。但回调方法ClientEffectApplied是个rpc方法，会在客户端执行。
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UGDAbilitySystemComponent::ClientEffectApplied);
}

void UGDAbilitySystemComponent::InitAbilityInfosFromSaveData(ULoadScreenSaveGame* SaveData)
{
	UAbilityInfo* AbilityInfos = UGDAbilitySystemLibrary::GetAbilityInfo(this);
	
	FGDGameplayTags& GameplayTags = FGDGameplayTags::Get();
	for (const FSavedAbility& SavedAbility : SaveData->SavedAbilities)
	{
		// 只有解锁且可用才能授予能力
		if (SavedAbility.AbilityStatus != GameplayTags.Abilities_Status_Unlocked && SavedAbility.AbilityStatus != GameplayTags.Abilities_Status_Equipped)
		{
			continue;
		}
		// 能力等级检查
		if (SavedAbility.AbilityLevel < 1)
		{
			continue;
		}
		// 能力检查
		if (!SavedAbility.GameplayAbility)
		{
			continue;
		}
		
		FGDAbilityInfo* AbilityInfo = AbilityInfos->FindAbilityInfoPtrForTag(SavedAbility.AbilityTag);
		if (!AbilityInfo)
		{
			GDLOG_W(TEXT("GDAbilitySystemComponent"), TEXT("AbilityInfo is nullptr"));
			continue;
		}
		AbilityInfo->InputTag = SavedAbility.AbilitySlot;
		AbilityInfo->StatusTag = SavedAbility.AbilityStatus;

		FGameplayAbilitySpec AbilitySpec = AbilityInfo->CreateAbilitySpec(SavedAbility.AbilityLevel);
		
		if (SavedAbility.AbilityType == GameplayTags.Abilities_Type_Offensive)
		{
			GiveAbility(AbilitySpec);
		}
		else if (SavedAbility.AbilityType == GameplayTags.Abilities_Type_Passive && SavedAbility.AbilityStatus == GameplayTags.Abilities_Status_Equipped)
		{
			GiveAbility(AbilitySpec);
			TryActivateAbility(AbilitySpec.Handle);
		}
	}
	bStartupAbilitiesGiven = true;
	OnAbilitiesGiven.Broadcast(this);
}

void UGDAbilitySystemComponent::AddSetupAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupAbilities)
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
}

void UGDAbilitySystemComponent::AddSetupPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupPassiveAbilities)
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
		OnAbilitiesGiven.Broadcast(this);
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
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle,AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UGDAbilitySystemComponent::OnAbilityInputReleased(FGameplayTag InputTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle,AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UGDAbilitySystemComponent::OnAbilityInputHeld(FGameplayTag InputTag)
{
	if (!InputTag.IsValid()) return;
	
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			// 在Held中触发能力，在一直按住的时候，也能不断的触发能力
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UGDAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	// 锁住能力列表，防止遍历过程中 可激活的能力 变成 不可激活
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
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

FGameplayTag UGDAbilitySystemComponent::GetTypeTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	// type标签是加到DynamicAbilityTags中的，所以遍历这个就行
	const FGameplayTag& InputTagPrefix = FGameplayTag::RequestGameplayTag("Abilities.Type");
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

FGameplayTag UGDAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
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

void UGDAbilitySystemComponent::InitAbilityInfos()
{
	UAbilityInfo* AbilityInfos = UGDAbilitySystemLibrary::GetAbilityInfo(this);

	for (FGDAbilityInfo& AbilityInfo : AbilityInfos->AbilityInfos)
	{
		// 如果SetupAbility也存在于技能树中，就设置技能树的InputTag、StatusTag
		if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityInfo.AbilityTag))
		{
			AbilityInfo.InputTag = UGDAbilitySystemLibrary::FindInputTagFromAbilitySpec(AbilitySpec).First();
			if (AbilityInfo.InputTag.IsValid())
			{
				AbilityInfo.StatusTag = FGDGameplayTags::Get().Abilities_Status_Equipped;
			}
			else
			{
				AbilityInfo.StatusTag = FGDGameplayTags::Get().Abilities_Status_Unlocked;
			}
			continue;
		}

		if (!AbilityInfo.AbilityTag.IsValid())
		{
			GDLOG_E(TEXT("GDCharacterHero"), TEXT("AbilityInfo Invalid"));
			continue;
		}

		if (AbilityInfo.LevelRequirement <= 1)
		{
			UGDGameplayAbility* GDAbilityCDO = AbilityInfo.Ability->GetDefaultObject<UGDGameplayAbility>();
			if (GDAbilityCDO && GDAbilityCDO->SetupInputTag.IsValid())
			{
				AbilityInfo.InputTag = GDAbilityCDO->SetupInputTag;
				AbilityInfo.StatusTag = FGDGameplayTags::Get().Abilities_Status_Equipped;
			}
			else
			{
				AbilityInfo.StatusTag = FGDGameplayTags::Get().Abilities_Status_Unlocked;
			}
			GiveAbility(AbilityInfo.CreateAbilitySpec());
		}
		else
		{
			AbilityInfo.StatusTag = FGDGameplayTags::Get().Abilities_Status_Locked;
		}
	}

	bStartupAbilitiesGiven = true;
	OnAbilitiesGiven.Broadcast(this);
}

void UGDAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag,
                                                                              bool bActivate)
{
	OnPassiveEffectActivated.Broadcast(AbilityTag, bActivate);
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
	for (FGDAbilityInfo& Info : AbilityInfo->AbilityInfos)
	{
		if (!Info.AbilityTag.IsValid())
		{
			continue;
		}
		if (Info.LevelRequirement > Level)
		{
			continue;
		}
		// 解锁能力资格
		if (Info.StatusTag == FGDGameplayTags::Get().Abilities_Status_Locked)
		{
			Info.StatusTag = FGDGameplayTags::Get().Abilities_Status_Eligible;
			ClientNotifyAbilityStatusChanged(Info.AbilityTag, Info.StatusTag, 1);
		}
	}
}

void UGDAbilitySystemComponent::ClientNotifyAbilityStatusChanged_Implementation(const FGameplayTag& AbilityTag,
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
		FGDAbilityInfo* Info = AbilityInfo->FindAbilityInfoPtrForTag(AbilityTag);
		AGDPlayerState* GDPlayerState = CastChecked<AGDPlayerState>(GetOwner());

		if (!Info)
		{
			GDLOG(TEXT("GDAbilitySystemComponent"), TEXT("Info is nullptr"));
		}
		if (GDPlayerState->GetPlayerLevel() >= Info->LevelRequirement && GDPlayerState->GetSpellPoints() > 0)
		{
			// 解锁能力
			if (Info->StatusTag == FGDGameplayTags::Get().Abilities_Status_Eligible)
			{
				GDPlayerState->AddToSpellPoints(-1);
				Info->StatusTag = FGDGameplayTags::Get().Abilities_Status_Unlocked;
				
				GiveAbility(Info->CreateAbilitySpec());
				
				ClientNotifyAbilityStatusChanged(AbilityTag, Info->StatusTag, 1);
			}
			// 升级能力
			else
			{
				FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
				if (!AbilitySpec) return;
				int32 AbilityLevel = AbilitySpec->Level + 1;
				// 最高9级
				if (AbilityLevel <= 9)
				{
					GDPlayerState->AddToSpellPoints(-1);
					ClearAbility(AbilitySpec->Handle);
					GiveAbility(Info->CreateAbilitySpec(AbilityLevel));

					ClientNotifyAbilityStatusChanged(AbilityTag, Info->StatusTag, AbilityLevel);
				}
			}
		}
	}
}

void UGDAbilitySystemComponent::ServerEquipAbility_Implementation(FGameplayTag AbilityTag, FGameplayTag InputTag)
{
	FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag);
	
	if (!AbilitySpec)
	{
		GDLOG_W(TEXT("GDAbilitySystemComponent"), TEXT("AbilitySpec is nullptr"));
		return;
	}
	
	UAbilityInfo* AbilityInfos = UGDAbilitySystemLibrary::GetAbilityInfo(this);
	FGDAbilityInfo* AbilityInfo = AbilityInfos->FindAbilityInfoPtrForTag(GetAbilityTagFromSpec(*AbilitySpec));
	if (!AbilityInfo)
	{
		GDLOG_W(TEXT("GDAbilitySystemComponent"), TEXT("AbilityInfo is nullptr"));
		return;
	}

	FGDGameplayTags GameplayTags = FGDGameplayTags::Get();
		
	// 处理装备能力
	if (InputTag.IsValid())
	{
		// 如果其它能力也有同样的InputTag，就删除它。
		FForEachAbility Delegate;
		Delegate.BindLambda([this, &InputTag, &AbilityTag](FGameplayAbilitySpec& TempSpec)
		{
			FGameplayTag TempAbilityTag = GetAbilityTagFromSpec(TempSpec);
			if (GetInputTagFromSpec(TempSpec) == InputTag && TempAbilityTag != AbilityTag)
			{
				// 卸下能力
				ServerEquipAbility(TempAbilityTag, FGameplayTag::EmptyTag);
			}
		});
		ForEachAbility(Delegate);
		
		// 更新 InputTag
		AbilityInfo->InputTag = InputTag;
		UGDAbilitySystemLibrary::ResetInputTagFromAbilitySpec(AbilitySpec, AbilityInfo->InputTag);

		if (AbilityInfo->StatusTag != GameplayTags.Abilities_Status_Equipped)
		{
			AbilityInfo->StatusTag = GameplayTags.Abilities_Status_Equipped;

			// 如果是被动能力，还需要激活一次
			if (GameplayTags.Abilities_Type_Passive == GetTypeTagFromSpec(*AbilitySpec))
			{
				TryActivateAbility(AbilitySpec->Handle);
				OnPassiveEffectActivated.Broadcast(AbilityTag, true);
			}
		}
	}
	// 处理卸下能力
	else
	{
		// 删除Input标签，修改Status标签
		UGDAbilitySystemLibrary::ResetInputTagFromAbilitySpec(AbilitySpec, FGameplayTag::EmptyTag);
		UGDAbilitySystemLibrary::ResetStatusTagFromAbilitySpec(AbilitySpec, GameplayTags.Abilities_Status_Unlocked);
		
		AbilityInfo->InputTag = FGameplayTag::EmptyTag;
		AbilityInfo->StatusTag = GameplayTags.Abilities_Status_Unlocked;
		
		// 如果是被动技能，还需要结束该技能
		if (GameplayTags.Abilities_Type_Passive == GetTypeTagFromSpec(*AbilitySpec))
		{
			OnPassiveAbilityDeactivated.Broadcast(AbilityTag);
			OnPassiveEffectActivated.Broadcast(AbilityTag, false);
			CancelAbilityHandle(AbilitySpec->Handle);
		}
	}
	
	ClientNotifyAbilityStatusChanged(AbilityInfo->AbilityTag, AbilityInfo->StatusTag, AbilitySpec->Level);
}