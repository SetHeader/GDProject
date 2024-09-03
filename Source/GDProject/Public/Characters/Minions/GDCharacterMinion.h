// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "../GDCharacterBase.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/EnemyInterface.h"
#include "GDCharacterMinion.generated.h"

class UWidgetComponent;
class UGDUserWidget;
class UGDWidgetController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeChangedSignature, float, NewValue);

/**
* 敌人角色，实现了GAS接口。
*/
UCLASS()
class GDPROJECT_API AGDCharacterMinion : public AGDCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "GDCharacterEnemy")
	FAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GDCharacterEnemy")
	FAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(VisibleAnywhere, Category = "GDCharacterEnemy")
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GDCharacterEnemy")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	int32 Level = 1.f;
	
public:
	AGDCharacterMinion();

	void BeginPlay() override;

	virtual void HighlightActor() override;

	virtual void UnHighlightActor() override;

	UFUNCTION(BlueprintCallable, Category = "CombatInterface")
	FORCEINLINE int32 GetPlayerLevel() const override { return Level; }
	
	virtual void BroadcastInitialValues() const;
	virtual void BindCallbacksToDependencies() const;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	virtual void InitializeAttributes() const override;
};
