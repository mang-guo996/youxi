// Copyright (C) 2025 契约象限. All Rights Reserved.
// 任务日志Widget

#pragma once

#include "CoreMinimal.h"
#include "UI/CQWidgetBase.h"
#include "Quest/CQQuestTypes.h"
#include "CQQuestLogWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UVerticalBox;

UCLASS()
class CONTRACTQUADRANT_API UCQQuestLogWidget : public UCQWidgetBase
{
	GENERATED_BODY()

public:
	// 刷新任务列表
	UFUNCTION(BlueprintCallable, Category = "任务日志")
	void RefreshQuestList();

	// 显示任务详情
	UFUNCTION(BlueprintCallable, Category = "任务日志")
	void ShowQuestDetail(const FCQQuestInstance& QuestInstance, const FCQQuestData& QuestData);

protected:
	// 活跃任务列表
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> ActiveQuestList;

	// 已完成任务列表
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> CompletedQuestList;

	// 任务详情
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_QuestTitle;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_QuestDescription;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> ObjectivesContainer;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> Txt_QuestReward;
};
