// Copyright (C) 2025 契约象限. All Rights Reserved.
// 任务数据结构

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Core/CQTypes.h"
#include "CQQuestTypes.generated.h"

// ============================================================================
// 任务目标
// ============================================================================
USTRUCT(BlueprintType)
struct FCQQuestObjective
{
	GENERATED_BODY()

	// 目标描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务")
	FText Description;

	// 目标类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务")
	ECQObjectiveType ObjectiveType = ECQObjectiveType::TalkToNPC;

	// 目标ID（NPC名、晶兽ID、物品ID等，根据类型不同含义不同）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务")
	FName TargetID;

	// 需要数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务")
	int32 RequiredCount = 1;

	// 当前完成数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务")
	int32 CurrentCount = 0;

	// 是否已完成
	bool IsCompleted() const { return CurrentCount >= RequiredCount; }
};

// ============================================================================
// 任务奖励
// ============================================================================
USTRUCT(BlueprintType)
struct FCQQuestReward
{
	GENERATED_BODY()

	// 金币奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "奖励")
	int32 Gold = 0;

	// 经验值奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "奖励")
	int32 Experience = 0;

	// 物品奖励（物品ID → 数量）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "奖励")
	TMap<FName, int32> Items;

	// 解锁晶兽ID（空则无）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "奖励")
	FName UnlockBeastID;
};

// ============================================================================
// 任务数据（DataTable行）
// ============================================================================
USTRUCT(BlueprintType)
struct FCQQuestData : public FTableRowBase
{
	GENERATED_BODY()

	// 任务ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FName QuestID;

	// 任务名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FText QuestName;

	// 任务描述
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	FText Description;

	// 是否为主线任务
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	bool bIsMainQuest = true;

	// 任务所在区域
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "基本")
	ECQWorldArea QuestArea = ECQWorldArea::CrystalTown;

	// 前置任务ID列表（全部完成才可接取）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "前置条件")
	TArray<FName> PrerequisiteQuestIDs;

	// 最低等级要求
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "前置条件")
	int32 MinLevelRequired = 1;

	// 任务目标列表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "目标")
	TArray<FCQQuestObjective> Objectives;

	// 任务奖励
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "奖励")
	FCQQuestReward Reward;

	// 接取NPC的ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName QuestGiverNPCID;

	// 交付NPC的ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName QuestTurnInNPCID;

	// 接取时的对话ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName AcceptDialogueID;

	// 完成时的对话ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName CompleteDialogueID;
};

// ============================================================================
// 任务运行时实例（包含状态）
// ============================================================================
USTRUCT(BlueprintType)
struct FCQQuestInstance
{
	GENERATED_BODY()

	// 任务ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务")
	FName QuestID;

	// 当前状态
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务")
	ECQQuestState State = ECQQuestState::Locked;

	// 运行时目标进度
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "任务")
	TArray<FCQQuestObjective> Objectives;

	// 是否所有目标都完成了
	bool AreAllObjectivesComplete() const
	{
		for (const FCQQuestObjective& Obj : Objectives)
		{
			if (!Obj.IsCompleted())
			{
				return false;
			}
		}
		return Objectives.Num() > 0;
	}
};
