// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "../GDCharacterBase.h"
#include "GDCharacterHero.generated.h"

class UInputMappingContext;
class AGDPlayerState;

/**
* 玩家角色，封装了增强输入功能，实现了GAS接口。
*/
UCLASS()
class GDPROJECT_API AGDCharacterHero : public AGDCharacterBase
{
	GENERATED_BODY()

public:
	/// @brief 初始时 拥有的输入
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GDCharacterHero")
	TArray<UInputMappingContext*> InitialIMCArray;

public:
	// ServerOnly	用来初始化数据
	void PossessedBy(AController* NewController) override;

	// ClientOnly	用来初始化UI
	void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintCallable, Category = "GDCharacterHero")
	void AddIMC(UInputMappingContext* IMC, int32 Priority = 0);

	UFUNCTION(BlueprintCallable, Category = "GDCharacterHero")
	void RemoveIMC(UInputMappingContext* IMC);

	UFUNCTION(BlueprintCallable, Category = "CombatInterface")
	FORCEINLINE int32 GetPlayerLevel() const override;

protected:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Server & Client 初始化gas系统
	void InitAbilityActorInfo();

public:
	/**
	 * ICombatInterface start
	 */
	virtual FVector GetCombatSocketLocation() override;
	/**
	 * ICombatInterface end
	 */
};
