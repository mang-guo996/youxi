// Copyright (C) 2025 契约象限. All Rights Reserved.
// 行为树装饰器：根据任务状态判断

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Core/CQTypes.h"
#include "CQBTDecorator_QuestState.generated.h"

UCLASS()
class CONTRACTQUADRANT_API UCQBTDecorator_QuestState : public UBTDecorator
{
	GENERATED_BODY()

public:
	UCQBTDecorator_QuestState();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	// 要检查的任务ID
	UPROPERTY(EditAnywhere, Category = "任务")
	FName QuestID;

	// 期望的任务状态
	UPROPERTY(EditAnywhere, Category = "任务")
	ECQQuestState ExpectedState = ECQQuestState::Active;
};
