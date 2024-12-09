#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySet_GameplayAbility.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UGameplayAbility;

// GameplayAbility能力
USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FAbilitySet_GameplayAbility {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	bool bActivateOnce = false;
};
// GameplayEffect能力
USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FAbilitySet_GameplayEffect {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float EffectLevel = 1.f;
};

/**
 * 能力集合
 */
USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FAbilitySet {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TArray<FAbilitySet_GameplayAbility> GrantAbilities;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Inventory")
	TArray<FAbilitySet_GameplayEffect> GrantEffects;
};

/**
* 授予能力后的句柄
*/
USTRUCT(BlueprintType)
struct INVENTORYMODULE_API FAbilitySet_GrantHandles {
	GENERATED_BODY()
public:
	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	// 清除ASC所有关于该Handle的能力和效果，并清空能力和效果数组
	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);
protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;
};