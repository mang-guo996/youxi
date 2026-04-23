// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "Quest/CQQuestManager.h"
#include "Engine/DataTable.h"
#include "ContractQuadrant.h"

void UCQQuestManager::Initialize()
{
	QuestInstances.Empty();
	CompletedQuests.Empty();

	UE_LOG(LogContractQuadrant, Log, TEXT("任务管理器初始化完成"));
}

// ============================================================================
// 任务操作
// ============================================================================

bool UCQQuestManager::AcceptQuest(FName QuestID)
{
	// 检查是否可以接取
	if (!CanAcceptQuest(QuestID))
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("无法接取任务: %s"), *QuestID.ToString());
		return false;
	}

	// 获取任务数据
	FCQQuestData* QuestData = GetQuestData(QuestID);
	if (!QuestData)
	{
		return false;
	}

	// 创建任务实例
	FCQQuestInstance Instance;
	Instance.QuestID = QuestID;
	Instance.State = ECQQuestState::Active;
	Instance.Objectives = QuestData->Objectives;

	// 重置目标进度
	for (FCQQuestObjective& Obj : Instance.Objectives)
	{
		Obj.CurrentCount = 0;
	}

	QuestInstances.Add(QuestID, Instance);
	OnQuestAccepted.Broadcast(QuestID);

	UE_LOG(LogContractQuadrant, Log, TEXT("接取任务: %s"), *QuestID.ToString());
	return true;
}

void UCQQuestManager::UpdateObjective(ECQObjectiveType ObjectiveType, FName TargetID, int32 Count)
{
	// 遍历所有活跃任务，更新匹配的目标
	for (auto& Pair : QuestInstances)
	{
		FCQQuestInstance& Instance = Pair.Value;
		if (Instance.State != ECQQuestState::Active)
		{
			continue;
		}

		for (int32 i = 0; i < Instance.Objectives.Num(); ++i)
		{
			FCQQuestObjective& Obj = Instance.Objectives[i];
			if (Obj.ObjectiveType == ObjectiveType && Obj.TargetID == TargetID && !Obj.IsCompleted())
			{
				Obj.CurrentCount = FMath::Min(Obj.CurrentCount + Count, Obj.RequiredCount);
				OnQuestObjectiveUpdated.Broadcast(Instance.QuestID, i);

				UE_LOG(LogContractQuadrant, Log, TEXT("任务 %s 目标更新: %s (%d/%d)"),
					*Instance.QuestID.ToString(), *TargetID.ToString(),
					Obj.CurrentCount, Obj.RequiredCount);

				// 检查是否所有目标完成
				if (Instance.AreAllObjectivesComplete())
				{
					UE_LOG(LogContractQuadrant, Log, TEXT("任务 %s 所有目标已完成！"), *Instance.QuestID.ToString());
				}
			}
		}
	}
}

bool UCQQuestManager::TryCompleteQuest(FName QuestID)
{
	FCQQuestInstance* Instance = QuestInstances.Find(QuestID);
	if (!Instance || Instance->State != ECQQuestState::Active)
	{
		return false;
	}

	if (!Instance->AreAllObjectivesComplete())
	{
		UE_LOG(LogContractQuadrant, Warning, TEXT("任务 %s 目标未全部完成"), *QuestID.ToString());
		return false;
	}

	Instance->State = ECQQuestState::Completed;
	CompletedQuests.Add(QuestID);
	OnQuestCompleted.Broadcast(QuestID);

	UE_LOG(LogContractQuadrant, Log, TEXT("任务完成: %s"), *QuestID.ToString());
	return true;
}

void UCQQuestManager::AbandonQuest(FName QuestID)
{
	FCQQuestInstance* Instance = QuestInstances.Find(QuestID);
	if (Instance && Instance->State == ECQQuestState::Active)
	{
		Instance->State = ECQQuestState::Failed;
		OnQuestFailed.Broadcast(QuestID);
		UE_LOG(LogContractQuadrant, Log, TEXT("放弃任务: %s"), *QuestID.ToString());
	}
}

// ============================================================================
// 查询接口
// ============================================================================

ECQQuestState UCQQuestManager::GetQuestState(FName QuestID) const
{
	const FCQQuestInstance* Instance = QuestInstances.Find(QuestID);
	if (Instance)
	{
		return Instance->State;
	}

	// 检查是否满足前置条件
	if (ArePrerequisitesMet(QuestID))
	{
		return ECQQuestState::Available;
	}

	return ECQQuestState::Locked;
}

TArray<FCQQuestInstance> UCQQuestManager::GetActiveQuests() const
{
	TArray<FCQQuestInstance> ActiveQuests;
	for (const auto& Pair : QuestInstances)
	{
		if (Pair.Value.State == ECQQuestState::Active)
		{
			ActiveQuests.Add(Pair.Value);
		}
	}
	return ActiveQuests;
}

TArray<FName> UCQQuestManager::GetCompletedQuestIDs() const
{
	return CompletedQuests;
}

TArray<FName> UCQQuestManager::GetAvailableQuestIDs() const
{
	TArray<FName> Available;

	if (!QuestDataTable)
	{
		return Available;
	}

	TArray<FCQQuestData*> AllQuests;
	QuestDataTable->GetAllRows<FCQQuestData>(TEXT("AvailableQuests"), AllQuests);

	for (const FCQQuestData* Quest : AllQuests)
	{
		if (!QuestInstances.Contains(Quest->QuestID) &&
			!CompletedQuests.Contains(Quest->QuestID) &&
			CheckPrerequisites(Quest))
		{
			Available.Add(Quest->QuestID);
		}
	}

	return Available;
}

TArray<FName> UCQQuestManager::GetActiveQuestIDs() const
{
	TArray<FName> ActiveIDs;
	for (const auto& Pair : QuestInstances)
	{
		if (Pair.Value.State == ECQQuestState::Active)
		{
			ActiveIDs.Add(Pair.Key);
		}
	}
	return ActiveIDs;
}

bool UCQQuestManager::CanAcceptQuest(FName QuestID) const
{
	// 已接取或已完成的不能再接
	if (QuestInstances.Contains(QuestID) || CompletedQuests.Contains(QuestID))
	{
		return false;
	}

	return ArePrerequisitesMet(QuestID);
}

bool UCQQuestManager::ArePrerequisitesMet(FName QuestID) const
{
	FCQQuestData* QuestData = GetQuestData(QuestID);
	if (!QuestData)
	{
		return false;
	}

	return CheckPrerequisites(QuestData);
}

FCQQuestInstance UCQQuestManager::GetQuestInstance(FName QuestID) const
{
	const FCQQuestInstance* Instance = QuestInstances.Find(QuestID);
	if (Instance)
	{
		return *Instance;
	}
	return FCQQuestInstance();
}

void UCQQuestManager::LoadQuestProgress(const TArray<FName>& CompletedIDs, const TArray<FName>& ActiveIDs)
{
	CompletedQuests = CompletedIDs;

	for (const FName& QuestID : ActiveIDs)
	{
		FCQQuestData* QuestData = GetQuestData(QuestID);
		if (QuestData)
		{
			FCQQuestInstance Instance;
			Instance.QuestID = QuestID;
			Instance.State = ECQQuestState::Active;
			Instance.Objectives = QuestData->Objectives;
			QuestInstances.Add(QuestID, Instance);
		}
	}

	UE_LOG(LogContractQuadrant, Log, TEXT("任务进度加载完成: 已完成%d个, 进行中%d个"),
		CompletedQuests.Num(), ActiveIDs.Num());
}

// ============================================================================
// 内部方法
// ============================================================================

FCQQuestData* UCQQuestManager::GetQuestData(FName QuestID) const
{
	if (!QuestDataTable)
	{
		return nullptr;
	}

	return QuestDataTable->FindRow<FCQQuestData>(QuestID, TEXT("QuestLookup"));
}

bool UCQQuestManager::CheckPrerequisites(const FCQQuestData* QuestData) const
{
	if (!QuestData)
	{
		return false;
	}

	for (const FName& PrereqID : QuestData->PrerequisiteQuestIDs)
	{
		if (!CompletedQuests.Contains(PrereqID))
		{
			return false;
		}
	}

	return true;
}
