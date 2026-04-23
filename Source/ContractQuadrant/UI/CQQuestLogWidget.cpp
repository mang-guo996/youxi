// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "UI/CQQuestLogWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "ContractQuadrant.h"

void UCQQuestLogWidget::RefreshQuestList()
{
	UE_LOG(LogContractQuadrant, Log, TEXT("刷新任务日志"));
	// 任务列表项的动态创建由蓝图处理
}

void UCQQuestLogWidget::ShowQuestDetail(const FCQQuestInstance& QuestInstance, const FCQQuestData& QuestData)
{
	if (Txt_QuestTitle)
	{
		Txt_QuestTitle->SetText(QuestData.QuestName);
	}

	if (Txt_QuestDescription)
	{
		Txt_QuestDescription->SetText(QuestData.Description);
	}

	// 显示目标进度
	if (ObjectivesContainer)
	{
		ObjectivesContainer->ClearChildren();

		for (const FCQQuestObjective& Obj : QuestInstance.Objectives)
		{
			FString ObjText = FString::Printf(TEXT("%s %s (%d/%d)"),
				Obj.IsCompleted() ? TEXT("✓") : TEXT("○"),
				*Obj.Description.ToString(),
				Obj.CurrentCount,
				Obj.RequiredCount);

			UE_LOG(LogContractQuadrant, Log, TEXT("  目标: %s"), *ObjText);
			// 动态创建文本Widget由蓝图处理
		}
	}

	// 显示奖励
	if (Txt_QuestReward)
	{
		FString RewardText;
		if (QuestData.Reward.Gold > 0)
		{
			RewardText += FString::Printf(TEXT("金币: %d  "), QuestData.Reward.Gold);
		}
		if (QuestData.Reward.Experience > 0)
		{
			RewardText += FString::Printf(TEXT("经验: %d  "), QuestData.Reward.Experience);
		}
		Txt_QuestReward->SetText(FText::FromString(RewardText));
	}
}
