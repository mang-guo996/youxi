// Copyright (C) 2025 契约象限. All Rights Reserved.
// 行为树任务节点：NPC交互（面向玩家等）

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CQBTTask_Interact.generated.h"

UCLASS()
class CONTRACTQUADRANT_API UCQBTTask_Interact : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCQBTTask_Interact();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
