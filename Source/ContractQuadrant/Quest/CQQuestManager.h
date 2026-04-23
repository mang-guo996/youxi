// Copyright (C) 2025 契约象限. All Rights Reserved.
// 任务管理器

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest/CQQuestTypes.h"
#include "CQQuestManager.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccepted, FName, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestObjectiveUpdated, FName, QuestID, int32, ObjectiveIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, FName, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestFailed, FName, QuestID);

UCLASS(BlueprintType)
class CONTRACTQUADRANT_API UCQQuestManager : public UObject
{
	GENERATED_BODY()

public:
	// 初始化（加载任务DataTable）
	void Initialize();

	// ========== 任务操作 ==========

	// 接取任务
	UFUNCTION(BlueprintCallable, Category = "任务")
	bool AcceptQuest(FName QuestID);

	// 更新任务目标进度
	UFUNCTION(BlueprintCallable, Category = "任务")
	void UpdateObjective(ECQObjectiveType ObjectiveType, FName TargetID, int32 Count = 1);

	// 尝试完成任务
	UFUNCTION(BlueprintCallable, Category = "任务")
	bool TryCompleteQuest(FName QuestID);

	// 放弃任务
	UFUNCTION(BlueprintCallable, Category = "任务")
	void AbandonQuest(FName QuestID);

	// ========== 查询接口 ==========

	// 获取任务状态
	UFUNCTION(BlueprintCallable, Category = "任务")
	ECQQuestState GetQuestState(FName QuestID) const;

	// 获取所有活跃任务
	UFUNCTION(BlueprintCallable, Category = "任务")
	TArray<FCQQuestInstance> GetActiveQuests() const;

	// 获取已完成任务列表
	UFUNCTION(BlueprintCallable, Category = "任务")
	TArray<FName> GetCompletedQuestIDs() const;

	// 获取可接取任务列表
	UFUNCTION(BlueprintCallable, Category = "任务")
	TArray<FName> GetAvailableQuestIDs() const;

	// 获取活跃任务ID列表
	TArray<FName> GetActiveQuestIDs() const;

	// 检查任务是否可接取
	UFUNCTION(BlueprintCallable, Category = "任务")
	bool CanAcceptQuest(FName QuestID) const;

	// 检查前置任务是否满足
	UFUNCTION(BlueprintCallable, Category = "任务")
	bool ArePrerequisitesMet(FName QuestID) const;

	// 获取任务实例数据
	UFUNCTION(BlueprintCallable, Category = "任务")
	FCQQuestInstance GetQuestInstance(FName QuestID) const;

	// 从存档加载任务进度
	void LoadQuestProgress(const TArray<FName>& CompletedIDs, const TArray<FName>& ActiveIDs);

	// ========== 委托 ==========

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnQuestAccepted OnQuestAccepted;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnQuestObjectiveUpdated OnQuestObjectiveUpdated;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnQuestCompleted OnQuestCompleted;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnQuestFailed OnQuestFailed;

	// ========== 配置 ==========

	// 任务数据表
	UPROPERTY(EditDefaultsOnly, Category = "数据")
	TObjectPtr<UDataTable> QuestDataTable;

private:
	// 所有任务实例（运行时）
	UPROPERTY()
	TMap<FName, FCQQuestInstance> QuestInstances;

	// 已完成的任务ID
	UPROPERTY()
	TArray<FName> CompletedQuests;

	// 获取任务基础数据
	FCQQuestData* GetQuestData(FName QuestID) const;

	// 检查前置任务
	bool CheckPrerequisites(const FCQQuestData* QuestData) const;
};
