// Fill out your copyright notice in the Description page of Project Settings.


#include "OurAssetEditorToolkit.h"

void FOurAssetEditorToolkit::InitializeAssetEditor(const EToolkitMode::Type Mode,
	const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InAssets)
{
	// InitAssetEditor(Mode, InitToolkitHost, FName("OurAssetEditor"), StandaloneOurAssetEditor, true, true, InAssets);
	RegenerateMenusAndToolbars();
}
