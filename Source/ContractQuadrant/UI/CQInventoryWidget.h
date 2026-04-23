// Copyright (C) 2025 契约象限. All Rights Reserved.
// 背包界面Widget

#pragma once

#include "CoreMinimal.h"
#include "UI/CQWidgetBase.h"
#include "Core/CQTypes.h"
#include "CQInventoryWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UButton;
class UImage;

UCLASS()
class CONTRACTQUADRANT_API UCQInventoryWidget : public UCQWidgetBase
{
	GENERATED_BODY()

public:
	// 刷新背包列表
	UFUNCTION(BlueprintCallable, Category = "背包")
	void RefreshInventory();

	// 使用物品
	UFUNCTION(BlueprintCallable, Category = "背包")
	void UseItem(FName ItemID);

	// 显示物品详情
	UFUNCTION(BlueprintCallable, Category = "背包")
	void ShowItemDetail(const FCQItemData& ItemData, int32 Count);

protected:
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> ItemListScrollBox;

	// 物品详情面板
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_ItemName;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_ItemDescription;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_ItemCount;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> Img_ItemIcon;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_UseItem;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> Btn_DiscardItem;

	// 金币显示
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_Gold;
};
