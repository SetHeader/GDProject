// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class CUSTOMEDITOR_API FAssetTypeActions_OurAssets : public FAssetTypeActions_Base
{
private:
	EAssetTypeCategories::Type MyAssetCategory;
	
public:
	FAssetTypeActions_OurAssets(EAssetTypeCategories::Type Type): MyAssetCategory(Type) {}
	
	virtual FText GetName() const override { return NSLOCTEXT("Editor", "Our Assets Name", "我们的资产"); }
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override { return FColor::Green; }
	virtual uint32 GetCategories() override { return MyAssetCategory; }

	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
};
