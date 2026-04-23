// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "AI/CQBTTask_Patrol.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UCQBTTask_Patrol::UCQBTTask_Patrol()
{
	NodeName = TEXT("巡逻");
	bNotifyTick = true;
}

EBTNodeResult::Type UCQBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn())
	{
		return EBTNodeResult::Failed;
	}

	FVector PatrolPoint = GetRandomPatrolPoint(AIController->GetPawn());

	// 移动到巡逻点
	EPathFollowingRequestResult::Type Result =
		AIController->MoveToLocation(PatrolPoint, AcceptanceRadius);

	if (Result == EPathFollowingRequestResult::RequestSuccessful ||
		Result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UCQBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	// 检查是否到达目标
	if (AIController->GetMoveStatus() != EPathFollowingStatus::Moving)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

FVector UCQBTTask_Patrol::GetRandomPatrolPoint(AActor* Owner) const
{
	FVector Origin = Owner->GetActorLocation();
	FNavLocation NavLocation;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(Owner->GetWorld());
	if (NavSystem && NavSystem->GetRandomReachablePointInRadius(Origin, PatrolRadius, NavLocation))
	{
		return NavLocation.Location;
	}

	// 导航系统不可用时，返回随机偏移点
	FVector RandomOffset = FMath::VRand() * PatrolRadius;
	RandomOffset.Z = 0;
	return Origin + RandomOffset;
}
