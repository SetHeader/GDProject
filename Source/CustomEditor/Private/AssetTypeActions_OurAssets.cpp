// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetTypeActions_OurAssets.h"

#include "OurAssetEditorToolkit.h"
#include "OurAssetsObject.h"

UClass* FAssetTypeActions_OurAssets::GetSupportedClass() const
{
	return UOurAssetsObject::StaticClass();
}

void FAssetTypeActions_OurAssets::OpenAssetEditor(const TArray<UObject*>& InObjects,
                                                  TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type ToolKitModeType = EditWithinLevelEditor ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	
	for (auto ObjectIterator = InObjects.CreateConstIterator(); ObjectIterator; ++ObjectIterator)
	{
		if (UOurAssetsObject* OurAsset = Cast<UOurAssetsObject>(*ObjectIterator))
		{
			const TSharedRef<FOurAssetEditorToolkit> RecoilAssetEditorToolKit = MakeShareable(new FOurAssetEditorToolkit());
			RecoilAssetEditorToolKit->InitializeAssetEditor(ToolKitModeType, EditWithinLevelEditor, OurAsset);
		}
	}
}
