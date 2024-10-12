// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "../GDCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "GDCharacterMinion.generated.h"

struct FScalableFloat;
class UWidgetComponent;
class UGDUserWidget;
class UGDWidgetController;
class UBehaviorTree;
class AGDAIController;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combat")
	FScalableFloat XPReward;
	
protected:
	int32 Level = 1.f;
	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TObjectPtr<AGDAIController> GDAIController;
	
public:
	AGDCharacterMinion();

	void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	virtual void SetCombatTarget_Implementation(AActor* Target) override;
	/** End Enemy Interface */

	/** Combat Interface */
	UFUNCTION(BlueprintCallable, Category = "CombatInterface")
	FORCEINLINE int32 GetPlayerLevel_Implementation() const { return Level; }
	virtual void Die() override;
	/** End Combat Interface */
	
	virtual void BroadcastInitialValues() const;
	virtual void BindCallbacksToDependencies() const;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

protected:
	virtual void InitializeAttributes() const override;

};
