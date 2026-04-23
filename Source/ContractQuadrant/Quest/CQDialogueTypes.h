// Copyright (C) 2025 契约象限. All Rights Reserved.
// 对话数据结构

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CQDialogueTypes.generated.h"

// ============================================================================
// 对话选项
// ============================================================================
USTRUCT(BlueprintType)
struct FCQDialogueOption
{
	GENERATED_BODY()

	// 选项文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FText OptionText;

	// 跳转到的节点ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName NextNodeID;

	// 前置条件：需完成的任务ID（空则无条件）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName RequiredQuestID;

	// 选择此选项后触发的动作类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName TriggerActionID;
};

// ============================================================================
// 对话节点
// ============================================================================
USTRUCT(BlueprintType)
struct FCQDialogueNode : public FTableRowBase
{
	GENERATED_BODY()

	// 节点ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName NodeID;

	// 对话所属组ID（一个NPC可能有多组对话）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName DialogueGroupID;

	// 说话人名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FText SpeakerName;

	// 说话人头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	TSoftObjectPtr<UTexture2D> SpeakerPortrait;

	// 对话文本
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FText DialogueText;

	// 对话选项（空则为自动推进到下一节点）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	TArray<FCQDialogueOption> Options;

	// 无选项时自动跳转的下一节点ID（空则对话结束）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName NextNodeID;

	// 节点进入时触发的动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName OnEnterActionID;

	// 节点退出时触发的动作
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName OnExitActionID;

	// 是否为对话终止节点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	bool bIsEndNode = false;
};
