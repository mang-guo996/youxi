// Copyright (C) 2025 契约象限. All Rights Reserved.
// 行为树服务：检测玩家距离

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CQBTService_DetectPlayer.generated.h"

UCLASS()
class CONTRACTQUADRANT_API UCQBTService_DetectPlayer : public UBTService
{
	GENERATED_BODY()

public:
	UCQBTService_DetectPlayer();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 检测范围
	UPROPERTY(EditAnywhere, Category = "检测")
	float DetectionRadius = 800.0f;

	// 近距离阈值（触发对话等交互）
	UPROPERTY(EditAnywhere, Category = "检测")
	float NearbyThreshold = 300.0f;
};
