// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class CUSTOMEDITOR_API FOurAssetEditorToolkit : public FAssetEditorToolkit
{
public:
	// 必须实现的接口
	virtual FName GetToolkitFName() const override { return FName("OurAssetsEditorToolkit"); }
	virtual FText GetBaseToolkitName() const override { return NSLOCTEXT("EditorExtension", "Out Asset Toolkit Name", "我们的资产编辑器"); }
	virtual FString GetWorldCentricTabPrefix() const override { return NSLOCTEXT("EditorExtension", "Out Asset Toolkit Tab Prefix", "我们的资产").ToString(); }
	virtual FLinearColor GetWorldCentricTabColorScale() const override { return FLinearColor::Green; }

	// virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	// virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

	// 这个函数并不是虚函数，也不含有模式匹配，为公开函数被外部调用
	void InitializeAssetEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UObject* InAssets);
};
