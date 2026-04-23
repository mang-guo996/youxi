// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Quest/CQDialogueComponent.h"
#include "Engine/DataTable.h"
#include "ContractQuadrant.h"

UCQDialogueComponent::UCQDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// ============================================================================
// 对话控制
// ============================================================================

bool UCQDialogueComponent::StartDialogue(FName DialogueGroupID)
{
	if (bIsInDialogue)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("已在对话中"));
		return false;
	}

	if (!DialogueDataTable)
	{
		UE_LOG(LogContractQuadrant, Error, TEXT("对话数据表未设置"));
		return false;
	}

	// 查找对话组的起始节点
	FCQDialogueNode* StartNode = FindDialogueStartNode(DialogueGroupID);
	if (!StartNode)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("未找到对话组: %s"), *DialogueGroupID.ToString());
		return false;
	}

	bIsInDialogue = true;
	CurrentDialogueGroupID = DialogueGroupID;
	CurrentNode = *StartNode;

	// 处理进入节点动作
	if (!CurrentNode.OnEnterActionID.IsNone())
	{
		ProcessNodeAction(CurrentNode.OnEnterActionID);
	}

	OnDialogueStarted.Broadcast(DialogueGroupID);
	OnDialogueNodeChanged.Broadcast(CurrentNode);

	UE_LOG(LogContractQuadrant, Log, TEXT("开始对话: %s"), *DialogueGroupID.ToString());
	return true;
}

void UCQDialogueComponent::AdvanceDialogue()
{
	if (!bIsInDialogue)
	{
		return;
	}

	// 处理退出当前节点动作
	if (!CurrentNode.OnExitActionID.IsNone())
	{
		ProcessNodeAction(CurrentNode.OnExitActionID);
	}

	// 检查是否为结束节点
	if (CurrentNode.bIsEndNode || CurrentNode.NextNodeID.IsNone())
	{
		EndDialogue();
		return;
	}

	// 导航到下一个节点
	if (!NavigateToNode(CurrentNode.NextNodeID))
	{
		EndDialogue();
	}
}

void UCQDialogueComponent::SelectOption(int32 OptionIndex)
{
	if (!bIsInDialogue)
	{
		return;
	}

	TArray<FCQDialogueOption> AvailableOptions = GetAvailableOptions();
	if (!AvailableOptions.IsValidIndex(OptionIndex))
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("无效的选项索引: %d"), OptionIndex);
		return;
	}

	const FCQDialogueOption& SelectedOption = AvailableOptions[OptionIndex];

	// 触发选项动作
	if (!SelectedOption.TriggerActionID.IsNone())
	{
		ProcessNodeAction(SelectedOption.TriggerActionID);
	}

	// 处理退出当前节点动作
	if (!CurrentNode.OnExitActionID.IsNone())
	{
		ProcessNodeAction(CurrentNode.OnExitActionID);
	}

	// 导航到选项指向的节点
	if (SelectedOption.NextNodeID.IsNone())
	{
		EndDialogue();
	}
	else
	{
		if (!NavigateToNode(SelectedOption.NextNodeID))
		{
			EndDialogue();
		}
	}
}

void UCQDialogueComponent::EndDialogue()
{
	if (!bIsInDialogue)
	{
		return;
	}

	bIsInDialogue = false;
	CurrentDialogueGroupID = NAME_None;

	OnDialogueEnded.Broadcast();
	UE_LOG(LogContractQuadrant, Log, TEXT("对话结束"));
}

// ============================================================================
// 查询接口
// ============================================================================

TArray<FCQDialogueOption> UCQDialogueComponent::GetAvailableOptions() const
{
	TArray<FCQDialogueOption> FilteredOptions;

	for (const FCQDialogueOption& Option : CurrentNode.Options)
	{
		// 检查前置条件
		if (Option.RequiredQuestID.IsNone())
		{
			FilteredOptions.Add(Option);
		}
		else
		{
			// TODO: 检查任务状态（需要访问QuestManager）
			// 目前先全部显示
			FilteredOptions.Add(Option);
		}
	}

	return FilteredOptions;
}

// ============================================================================
// 内部方法
// ============================================================================

bool UCQDialogueComponent::NavigateToNode(FName NodeID)
{
	if (!DialogueDataTable)
	{
		return false;
	}

	FCQDialogueNode* Node = DialogueDataTable->FindRow<FCQDialogueNode>(NodeID, TEXT("DialogueNav"));
	if (!Node)
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("未找到对话节点: %s"), *NodeID.ToString());
		return false;
	}

	CurrentNode = *Node;

	// 处理进入节点动作
	if (!CurrentNode.OnEnterActionID.IsNone())
	{
		ProcessNodeAction(CurrentNode.OnEnterActionID);
	}

	OnDialogueNodeChanged.Broadcast(CurrentNode);
	return true;
}

void UCQDialogueComponent::ProcessNodeAction(FName ActionID)
{
	OnDialogueActionTriggered.Broadcast(ActionID);
	UE_LOG(LogContractQuadrant, Log, TEXT("对话动作触发: %s"), *ActionID.ToString());

	// 具体动作处理（接取任务、给予物品等）由蓝图或订阅者响应
	// 常见动作ID约定：
	// "AcceptQuest_MQ_001" → 接取任务 MQ_001
	// "GiveItem_Potion_3" → 给予3个药水
	// "UnlockTeleport_Forest" → 解锁森林传送点
	// "StartBattle_Trainer01" → 触发训练师战斗
}

FCQDialogueNode* UCQDialogueComponent::FindDialogueStartNode(FName GroupID) const
{
	if (!DialogueDataTable)
	{
		return nullptr;
	}

	TArray<FCQDialogueNode*> AllNodes;
	DialogueDataTable->GetAllRows<FCQDialogueNode>(TEXT("FindStartNode"), AllNodes);

	for (FCQDialogueNode* Node : AllNodes)
	{
		if (Node->DialogueGroupID == GroupID)
		{
			return Node; // 返回该组的第一个节点作为起始
		}
	}

	return nullptr;
}
