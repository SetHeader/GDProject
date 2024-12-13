// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "../GDCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "Interface/InventoryManagerInferface.h"
#include "Interface/EquipmentManagerInferface.h"
#include "GDCharacterHero.generated.h"

class UPassiveNiagaraComponent;
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
class GDPROJECT_API AGDCharacterHero : public AGDCharacterBase, public IPlayerInterface, public IInventoryManagerInterface, public IEquipmentManagerInterface {
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

	// 几种被动效果的特效
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;

	// 被动效果特效的附加点
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;

	// 背包组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GDCharacterBase")
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	// 装备栏组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GDCharacterBase")
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
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

	/** Begin IInventoryManagerInterface */
	virtual UInventoryManagerComponent* GetInventoryManagerComponent() const override;
	/** End IInventoryManagerInterface */
	
	/** Begin IEquipmentManagerInterface */
	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent() const override;
	/** End IEquipmentManagerInterface */
	virtual void OnRep_IsCanAttack() override;
	
	UFUNCTION(Reliable, NetMulticast)
	void Multicast_LevelUpParticles() const;
	
	void LoadProgress();
protected:
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Server & Client 初始化gas系统
	void InitAbilityActorInfo();
};
