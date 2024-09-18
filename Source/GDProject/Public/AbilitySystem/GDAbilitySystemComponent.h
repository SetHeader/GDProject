// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GDAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectTagApplied, const FGameplayTagContainer&)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven, UGDAbilitySystemComponent*)
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&)

UCLASS()
class GDPROJECT_API UGDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	FOnEffectTagApplied OnEffectTagApplied;
	
	// 当初始能力被给予的时候会回调，客户端、服务端都会回调，且只在初始化时回调一次。
	FAbilitiesGiven AbilitiesGivenDelegate;
	// AbilitiesGivenDelegate是否回调过了
	bool bStartupAbilitiesGiven = false;
public:
	UFUNCTION()
	void OnAbilityActorInfoSet();
	
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle ActiveGEHandle);

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& SetupPassiveAbilities);
	
	void OnRep_ActivateAbilities() override;

	// 处理输入事件
	void OnAbilityInputPressed(FGameplayTag InputTag);
	void OnAbilityInputReleased(FGameplayTag InputTag);
	void OnAbilityInputHeld(FGameplayTag InputTag);
	// 提供给外部的遍历 可激活能力 的方法。能保障安全遍历。
	void ForEachAbility(const FForEachAbility& Delegate);

	FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec) const;
	FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec) const;
	// 升级属性
	void UpgradeAttributePoint(const FGameplayTag& Tag);
	UFUNCTION(Reliable, Server)
	void Server_UpgradeAttributePoint(const FGameplayTag& Tag);
};
