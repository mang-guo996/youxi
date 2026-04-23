// Copyright (C) 2025 契约象限. All Rights Reserved.

#include "AI/CQBTDecorator_QuestState.h"
#include "Core/CQGameInstance.h"
#include "Quest/CQQuestManager.h"
#include "AIController.h"

UCQBTDecorator_QuestState::UCQBTDecorator_QuestState()
{
	NodeName = TEXT("检查任务状态");
}

bool UCQBTDecorator_QuestState::CalculateRawConditionValue(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return false;
	}

	UWorld* World = AIController->GetWorld();
	if (!World)
	{
		return false;
	}

	// 获取GameInstance中的任务管理器
	UCQGameInstance* GI = Cast<UCQGameInstance>(World->GetGameInstance());
	if (!GI || !GI->QuestManager)
	{
		return false;
	}

	// 检查任务状态
	ECQQuestState CurrentState = GI->QuestManager->GetQuestState(QuestID);
	return CurrentState == ExpectedState;
}
