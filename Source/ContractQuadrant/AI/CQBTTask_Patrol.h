// Copyright (C) 2025 契约象限. All Rights Reserved.
// 行为树任务节点：巡逻

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CQBTTask_Patrol.generated.h"

UCLASS()
class CONTRACTQUADRANT_API UCQBTTask_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCQBTTask_Patrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 巡逻半径
	UPROPERTY(EditAnywhere, Category = "巡逻")
	float PatrolRadius = 500.0f;

	// 到达目标点的接受距离
	UPROPERTY(EditAnywhere, Category = "巡逻")
	float AcceptanceRadius = 50.0f;

private:
	FVector GetRandomPatrolPoint(AActor* Owner) const;
};
