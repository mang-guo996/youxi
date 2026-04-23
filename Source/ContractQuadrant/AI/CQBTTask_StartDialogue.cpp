// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "AI/CQBTTask_StartDialogue.h"
#include "AI/CQAIController.h"
#include "Quest/CQDialogueComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCQBTTask_StartDialogue::UCQBTTask_StartDialogue()
{
	NodeName = TEXT("开始对话");
}

EBTNodeResult::Type UCQBTTask_StartDialogue::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPawn())
	{
		return EBTNodeResult::Failed;
	}

	// 获取NPC上的对话组件
	UCQDialogueComponent* DialogueComp =
		AIController->GetPawn()->FindComponentByClass<UCQDialogueComponent>();

	if (!DialogueComp)
	{
		return EBTNodeResult::Failed;
	}

	// 确定使用的对话组ID
	FName TargetDialogueID = DialogueGroupID.IsNone() ?
		DialogueComp->DefaultDialogueGroupID : DialogueGroupID;

	// 面向玩家
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (BB)
	{
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

		// 设置对话状态
		BB->SetValueAsBool(ACQAIController::BB_IsTalking, true);
	}

	// 开始对话
	if (DialogueComp->StartDialogue(TargetDialogueID))
	{
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
