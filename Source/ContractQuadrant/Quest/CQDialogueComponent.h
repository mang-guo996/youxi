// Copyright (C) 2025 契约象限. All Rights Reserved.
// 对话组件 - 挂载在NPC上，管理对话树遍历

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Quest/CQDialogueTypes.h"
#include "CQDialogueComponent.generated.h"

class UDataTable;
class UCQQuestManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueStarted, FName, DialogueGroupID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDialogueEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueNodeChanged, const FCQDialogueNode&, CurrentNode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDialogueActionTriggered, FName, ActionID);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CONTRACTQUADRANT_API UCQDialogueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCQDialogueComponent();

	// ========== 对话控制 ==========

	// 开始对话（指定对话组ID）
	UFUNCTION(BlueprintCallable, Category = "对话")
	bool StartDialogue(FName DialogueGroupID);

	// 推进对话（无选项时使用）
	UFUNCTION(BlueprintCallable, Category = "对话")
	void AdvanceDialogue();

	// 选择对话选项
	UFUNCTION(BlueprintCallable, Category = "对话")
	void SelectOption(int32 OptionIndex);

	// 结束对话
	UFUNCTION(BlueprintCallable, Category = "对话")
	void EndDialogue();

	// ========== 查询接口 ==========

	// 是否正在对话中
	UFUNCTION(BlueprintCallable, Category = "对话")
	bool IsInDialogue() const { return bIsInDialogue; }

	// 获取当前对话节点
	UFUNCTION(BlueprintCallable, Category = "对话")
	FCQDialogueNode GetCurrentNode() const { return CurrentNode; }

	// 获取当前可用选项
	UFUNCTION(BlueprintCallable, Category = "对话")
	TArray<FCQDialogueOption> GetAvailableOptions() const;

	// ========== 委托 ==========

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnDialogueStarted OnDialogueStarted;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnDialogueEnded OnDialogueEnded;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnDialogueNodeChanged OnDialogueNodeChanged;

	UPROPERTY(BlueprintAssignable, Category = "事件")
	FOnDialogueActionTriggered OnDialogueActionTriggered;

	// ========== 配置 ==========

	// 对话数据表
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "数据")
	TObjectPtr<UDataTable> DialogueDataTable;

	// NPC展示名称
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FText NPCName;

	// NPC头像
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	TObjectPtr<UTexture2D> NPCPortrait;

	// 默认对话组ID（玩家靠近交互时使用的默认对话）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "对话")
	FName DefaultDialogueGroupID;

private:
	bool bIsInDialogue = false;
	FCQDialogueNode CurrentNode;
	FName CurrentDialogueGroupID;

	// 导航到指定节点
	bool NavigateToNode(FName NodeID);

	// 处理节点动作
	void ProcessNodeAction(FName ActionID);

	// 获取对话组的起始节点
	FCQDialogueNode* FindDialogueStartNode(FName GroupID) const;
};
