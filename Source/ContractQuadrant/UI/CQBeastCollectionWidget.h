// Copyright (C) 2025 契约象限. All Rights Reserved.
// 晶兽图鉴Widget

#pragma once

#include "CoreMinimal.h"
#include "UI/CQWidgetBase.h"
#include "CrystalBeast/CQBeastDataTypes.h"
#include "CQBeastCollectionWidget.generated.h"

class UTextBlock;
class UImage;
class UScrollBox;
class UProgressBar;

UCLASS()
class CONTRACTQUADRANT_API UCQBeastCollectionWidget : public UCQWidgetBase
{
	GENERATED_BODY()

public:
	// 刷新图鉴列表
	UFUNCTION(BlueprintCallable, Category = "图鉴")
	void RefreshCollection();

	// 显示晶兽详情
	UFUNCTION(BlueprintCallable, Category = "图鉴")
	void ShowBeastDetail(const FCQBeastBaseData& BaseData, bool bDiscovered);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> BeastListScrollBox;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_CollectionProgress;

	// 详情面板
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_BeastName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_BeastElement;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_BeastDescription;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_BeastRarity;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Img_BeastPortrait;

	// 属性条
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> PB_HP;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> PB_Attack;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> PB_Defense;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> PB_Speed;
};
