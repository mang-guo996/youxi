// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "AI/CQAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ContractQuadrant.h"

const FName ACQAIController::BB_PlayerActor = FName("PlayerActor");
const FName ACQAIController::BB_PlayerDistance = FName("PlayerDistance");
const FName ACQAIController::BB_IsPlayerNearby = FName("IsPlayerNearby");
const FName ACQAIController::BB_IsTalking = FName("IsTalking");
const FName ACQAIController::BB_PatrolTarget = FName("PatrolTarget");
const FName ACQAIController::BB_HasQuestForPlayer = FName("HasQuestForPlayer");

ACQAIController::ACQAIController()
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void ACQAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset && BehaviorTreeAsset->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BehaviorTreeComp->StartTree(*BehaviorTreeAsset);

		UE_LOG(LogContractQuadrant, Log, TEXT("NPC AI启动: %s"), *InPawn->GetName());
	}
}

void ACQAIController::OnUnPossess()
{
	BehaviorTreeComp->StopTree();
	Super::OnUnPossess();
}

void ACQAIController::SetIsTalking(bool bTalking)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(BB_IsTalking, bTalking);
	}
}
