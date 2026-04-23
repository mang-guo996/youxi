// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "AI/CQBTTask_Interact.h"
#include "AI/CQAIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCQBTTask_Interact::UCQBTTask_Interact()
{
	NodeName = TEXT("NPC交互");
}

EBTNodeResult::Type UCQBTTask_Interact::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!AIController || !AIController->GetPawn() || !BB)
	{
		return EBTNodeResult::Failed;
	}

	// 面向玩家
	AActor* PlayerActor = Cast<AActor>(BB->GetValueAsObject(ACQAIController::BB_PlayerActor));
	if (PlayerActor)
	{
		FVector Direction = PlayerActor->GetActorLocation() - AIController->GetPawn()->GetActorLocation();
		Direction.Z = 0;
		if (!Direction.IsNearlyZero())
		{
			FRotator LookAtRotation = Direction.Rotation();
			AIController->GetPawn()->SetActorRotation(LookAtRotation);
		}
	}

	// 停止移动
	AIController->StopMovement();

	return EBTNodeResult::Succeeded;
}
