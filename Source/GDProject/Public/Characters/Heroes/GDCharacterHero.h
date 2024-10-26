// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "../GDCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "GDCharacterHero.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UNiagaraComponent;
class UInputMappingContext;
class AGDPlayerState;
class UDebuffNiagaraComponent;

/**
* 玩家角色，封装了增强输入功能，实现了GAS接口。
*/
UCLASS()
class GDPROJECT_API AGDCharacterHero : public AGDCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	/// @brief 初始时 拥有的输入
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GDCharacterHero")
	TArray<UInputMappingContext*> InitialIMCArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GDCharacterHero")
	TObjectPtr<UNiagaraComponent> LevelUpNiagraComponent;

	UPROPERTY(VisibleAnywhere, Category = "GDCharacterHero")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category = "GDCharacterHero")
	TObjectPtr<UCameraComponent> CameraComponent;
public:
	AGDCharacterHero();
	
	// ServerOnly	用来初始化数据
	void PossessedBy(AController* NewController) override;

	// ClientOnly	用来初始化UI
	void OnRep_PlayerState() override;
	
	UFUNCTION(BlueprintCallable, Category = "GDCharacterHero")
	void AddIMC(UInputMappingContext* IMC, int32 Priority = 0);

	UFUNCTION(BlueprintCallable, Category = "GDCharacterHero")
	void RemoveIMC(UInputMappingContext* IMC);


	/** Combat Interface */
	virtual int32 GetPlayerLevel_Implementation() const override;
	/** End Combat Interface */
	
	/** Player Interface */
	FORCEINLINE virtual int32 GetXP_Implementation() override;
	void AddToXP_Implementation(int32 InXP) override;
	virtual int32 FindLevelForXP_Implementation(int32 XP) override;
	virtual int32 GetAttributePointsReward_Implementation(const int32 InLevel) const override;
	virtual int32 GetSpellPointsReward_Implementation(const int32 InLevel) const override;
	
	virtual int32 GetAttributePoints_Implementation() override;
	virtual int32 GetSpellPoints_Implementation() override;
	virtual void AddToPlayerLevel_Implementation(int32 InLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InPoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InPoints) override;
	virtual void LevelUp_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	/** End Player Interface */

	UFUNCTION(Reliable, NetMulticast)
	void Multicast_LevelUpParticles() const;
	
	void LoadProgress();
protected:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Server & Client 初始化gas系统
	void InitAbilityActorInfo();

	// 初始化技能信息
	void InitAbilityInfos();
};
