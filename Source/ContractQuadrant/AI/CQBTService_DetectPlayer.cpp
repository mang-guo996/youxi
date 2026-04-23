// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "AI/CQBTService_DetectPlayer.h"
#include "AI/CQAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UCQBTService_DetectPlayer::UCQBTService_DetectPlayer()
{
	NodeName = TEXT("检测玩家");
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UCQBTService_DetectPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!AIController || !Blackboard || !AIController->GetPawn())
	{
		return;
	}

	// 获取玩家
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(AIController->GetWorld(), 0);
	if (!PlayerPawn)
	{
		Blackboard->SetValueAsFloat(ACQAIController::BB_PlayerDistance, -1.0f);
		Blackboard->SetValueAsBool(ACQAIController::BB_IsPlayerNearby, false);
		return;
	}

	// 计算距离
	float Distance = FVector::Dist(AIController->GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation());

	// 更新黑板
	Blackboard->SetValueAsObject(ACQAIController::BB_PlayerActor, PlayerPawn);
	Blackboard->SetValueAsFloat(ACQAIController::BB_PlayerDistance, Distance);
	Blackboard->SetValueAsBool(ACQAIController::BB_IsPlayerNearby, Distance <= NearbyThreshold);
}
