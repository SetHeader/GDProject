// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/GDWidgetController.h"
#include "GameplayTagContainer.h"
#include "GDOverlayWidgetController.generated.h"

struct FGDAbilityInfo;
class UGDAbilitySystemComponent;
class UAbilityInfo;
struct FOnAttributeChangeData;
class UGDUserWidget;
class UTexture2D;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIWidgetRow")
	FGameplayTag MessageTag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIWidgetRow")
	FText Text;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIWidgetRow")
	TSubclassOf<UGDUserWidget> MessageWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UIWidgetRow")
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FGDAbilityInfo&, Info);


UCLASS(BlueprintType, Blueprintable)
class GDPROJECT_API UGDOverlayWidgetController : public UGDWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnMaxManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnStaminaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnMaxStaminChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnPlayerLevelChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnAttributePointsChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FOnAttributeChangedSignature OnSpellPointsChangedDelegate;
	
	UPROPERTY(EditAnywhere, Category = "GDOverlayWidgetController")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditAnywhere, Category = "GDOverlayWidgetController")
	TObjectPtr<UAbilityInfo> AbilityInfoAsset;
	
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FMessageWidgetRowSignature OnMessageWidgetRow;
	
	UPROPERTY(BlueprintAssignable, Category = "GDOverlayWidgetController")
	FAbilityInfoSignature OnAbilityInfoDelegate;
public:
	// 在客户端处理
	void BindCallbacksToDependencies() const override;
	// 在客户端处理
	void BroadcastInitialValues() const override;
	// 在客户端处理
	void OnInitializeStartupAbilities(UGDAbilitySystemComponent* ASC) const;

	void HealthChanged(const FOnAttributeChangeData& ChangedData) const;
	void MaxHealthChanged(const FOnAttributeChangeData& ChangedData) const;
	void ManaChanged(const FOnAttributeChangeData& ChangedData) const;
	void MaxManaChanged(const FOnAttributeChangeData& ChangedData) const;
	void StaminaChanged(const FOnAttributeChangeData& ChangedData) const;
	void MaxStaminaChanged(const FOnAttributeChangeData& ChangedData) const;

	void OnXPChanged(int32 NewXP) const;

	UFUNCTION(BlueprintCallable, Category = "GDOverlayWidgetController")
	FGDAbilityInfo FindAbilityInfoByInputTag(FGameplayTag InputTag) const;
};
