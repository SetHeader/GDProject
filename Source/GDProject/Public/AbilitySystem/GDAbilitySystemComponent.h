// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GDAbilitySystemComponent.generated.h"


class ULoadScreenSaveGame;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectTagApplied, const FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UGDAbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility, FGameplayAbilitySpec&)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/)
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputTagPressed, const FGameplayTag& /*InputTag*/);

UCLASS()
class GDPROJECT_API UGDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	FOnEffectTagApplied OnEffectTagApplied;
	
	// 当初始能力被给予的时候会回调，客户端、服务端都会回调，且只在初始化时回调一次。
	FAbilitiesGiven OnAbilitiesGiven;
	// AbilitiesGivenDelegate是否回调过了
	bool bStartupAbilitiesGiven = false;

	FAbilityStatusChanged OnAbilityStatusChangedDelegate;

	// 装备能力的回调
	FAbilityEquipped OnAbilityEquipped;
	// 卸下被动能力的回调
	FDeactivatePassiveAbility OnPassiveAbilityDeactivated;
	// 激活被动能力的回调
	FActivatePassiveEffect OnPassiveEffectActivated;

	FOnInputTagPressed OnInputTagPressedDelegate;
	
public:
	UFUNCTION()
	void OnAbilityActorInfoSet();
	
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);

	// 初始化技能信息
	void InitAbilityInfos();
	
	// 从存档中加载并初始化技能信息
	void InitAbilityInfosFromSaveData(ULoadScreenSaveGame* SaveData);
	
	// 添加初始能力
	void AddSetupAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupAbilities);
	// 添加初始被动能力
	void AddSetupPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupPassiveAbilities);
	
	void OnRep_ActivateAbilities() override;

	// 处理输入事件
	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);
	void OnAbilityInputHeld(FGameplayTag InputTag);
	// 提供给外部的遍历 可激活能力 的方法。能保障安全遍历。
	void ForEachAbility(const FForEachAbility& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetTypeTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	// 获取能力的插槽标签。注：Slot Tag 就是 Input Tag
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag);
	
	/* 技能插槽相关 */
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	static void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);
	
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);
	
	// 升级属性
	void UpgradeAttributePoint(const FGameplayTag& Tag);
	UFUNCTION(Reliable, Server)
	void ServerUpgradeAttributePoint(const FGameplayTag& Tag);

	// 根据角色等级，来更新技能树的技能状态。即是否要解锁某个技能
	void UpdateAbilityStatus(int32 Level);

	// 技能状态变化后，通知给客户端
	UFUNCTION(Reliable, Client)
	void ClientNotifyAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);

	// 升级能力
	UFUNCTION(Reliable, Server)
	void ServerUpgradeAbility(FGameplayTag AbilityTag);

	// 装备能力，如果InputTag为空，则为卸下能力
	UFUNCTION(Reliable, Server)
	void ServerEquipAbility(FGameplayTag AbilityTag, FGameplayTag InputTag);
};
