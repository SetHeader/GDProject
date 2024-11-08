// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"

struct FGameplayAbilitySpec;
class UGameplayAbility;

/**
 * 用于角色的能力信息定义
 */
USTRUCT(BlueprintType)
struct FGDAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag = FGameplayTag();
	// 输入标签是会变化的，不能直接配置，要动态的查找出来
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag = FGameplayTag();
	
	// 技能状态，是否解锁、是否可用、是否已装备
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> BackgroundMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 0;
	// 对应的能力类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;

	FGameplayAbilitySpec CreateAbilitySpec(int32 AbilityLevel = 1) const;
};

/**
 * 这就是角色的技能树，角色的所有技能都存储在这里，包含已解锁、未解锁、已装备等技能。
 * 技能在解锁后就会添加到角色ASC身上，但只在装备时才会分配 输入标签。其中被动技能在装备时还会激活一次，但不会移除。
 * 涉及流程：
 * 一、第一次加载游戏
 * 1、注册CharacterBase类中的SetupAbilities和SetupPassiveAbilities，这两个不在技能树里面。（GDCharacterBase）
 * 2、根据技能树的初始信息注册所有技能。如果技能所需等为1，就直接解锁技能，并赋予给角色身上。（GDAbilitySystemComponent）
 * 3、角色升级时，满足技能所需条件，就获取解锁资格，但仍需要用一个技能点来解锁技能。（GDAbilitySystemComponent）
 * 4、使用技能点解锁技能后，技能会赋予给角色身上。（GDAbilitySystemComponent）
 * 5、装备技能后，技能会被分配InputTag，其中被动技能会触发一次。（GDAbilitySystemComponent）
 * 6、卸下技能时，技能会被分配空的InputTag，其中被动技能会结束触发。（GDAbilitySystemComponent）
 * 7、存档时，直接存储技能树即可。
 * 
 * 二、从存档中读取游戏
 * 1、注册CharacterBase类中的SetupAbilities和SetupPassiveAbilities，这两个不在技能树里面。（GDCharacterBase）
 * 2、从存档中读取技能树
 * 3、根据技能树的信息注册所有技能。
 * 4、其它同一的4、5、6、7
 */
UCLASS()
class GDPROJECT_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AbilityInformation")
	TArray<FGDAbilityInfo> AbilityInfos;

	FGDAbilityInfo FindAbilityInfoForTag(const FGameplayTag& Tag, bool bLogNotFound = false) const;

	FGDAbilityInfo* FindAbilityInfoPtrForTag(const FGameplayTag& Tag, bool bLogNotFound = false);
};
