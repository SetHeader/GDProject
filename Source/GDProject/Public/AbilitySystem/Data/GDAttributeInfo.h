
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GDAttributeInfo.generated.h"


USTRUCT(BlueprintType)
struct FAttributeInfo {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDesc;

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

UCLASS(BlueprintType)
class UGDAttributeInfo : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAttributeInfo> AttributeInfos;

	const FAttributeInfo NoneAttribute;

public:
	const FAttributeInfo& FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
};