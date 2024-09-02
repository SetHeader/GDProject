// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "../GDCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "GDCharacterMinion.generated.h"


/**
* 敌人角色，实现了GAS接口。
*/
UCLASS()
class GDPROJECT_API AGDCharacterMinion : public AGDCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

protected:
	int32 Level = 1.f;

public:
	AGDCharacterMinion();

	void BeginPlay() override;

	virtual void HighlightActor() override;

	virtual void UnHighlightActor() override;

	UFUNCTION(BlueprintCallable, Category = "CombatInterface")
	FORCEINLINE int32 GetPlayerLevel() const override { return Level; }
};
