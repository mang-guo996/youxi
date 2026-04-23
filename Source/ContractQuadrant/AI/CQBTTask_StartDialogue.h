// Copyright (C) 2025 契约象限. All Rights Reserved.
// 行为树任务节点：开始对话

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CQBTTask_StartDialogue.generated.h"

UCLASS()
class CONTRACTQUADRANT_API UCQBTTask_StartDialogue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCQBTTask_StartDialogue();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 要启动的对话组ID（空则使用NPC默认对话）
	UPROPERTY(EditAnywhere, Category = "对话")
	FName DialogueGroupID;
};
